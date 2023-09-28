#include "Enemies/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Soul.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
	HealthBar->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor Component"));
	PawnSensor->SightRadius = 4000.0f;
	PawnSensor->SetPeripheralVisionAngle(45.0f);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	//StartChasing();
	if (IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
		StartChasing();
	}
	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, const AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (!IsDead()) ToggleHealthBarVisibility(true);
	ClearPatrolTimer();
	ClearAttackTimer();
	ToggleWeaponCollision(ECollisionEnabled::NoCollision);
	StopAttackMontage();
	if (IsInsideAttackRadius())
	{
		if (!IsDead())StartAttackTimer();
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensor)
	{
		PawnSensor->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}
	InitEnemy();
	Tags.Add("Enemy");
}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

void AEnemy::AttackEnd()
{
	Super::AttackEnd();
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBar)
	{
		HealthBar->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void AEnemy::OnDeath()
{
	Super::OnDeath();
	EnemyState = EEnemyState::EES_Dead;
	ClearAttackTimer();
	ToggleHealthBarVisibility(false);
	DisableCapsule();
	SetLifeSpan(DeathTimeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	ToggleWeaponCollision(ECollisionEnabled::NoCollision);
	SpawnSoul();
}

void AEnemy::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass && Attributes)
	{
		ASoul* NewSoul = World->SpawnActor<ASoul>(SoulClass, GetActorLocation(), GetActorRotation());
		if (NewSoul)
		{
			NewSoul->SetSoulWorth(Attributes->GetSouls());
		}
	}
}


void AEnemy::InitEnemy()
{
	EnemyController = Cast<AAIController>(GetController());
	ToggleHealthBarVisibility(false);
	SpawnInitialPatrolTarget();
	SpawnDefaultWeapon();
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = DefaultWeapon;

	}
}

void AEnemy::SpawnInitialPatrolTarget()
{
	if (CurrentPatrolTarget == nullptr)
	{
		CurrentPatrolTarget = ChoosePatrolTarget();
	}
	MoveToTarget(CurrentPatrolTarget);
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("Player")) &&
		!SeenPawn->ActorHasTag(FName("Dead"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		StartChasing();
	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged())
		{
			StartPartrolling();
		}
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged())
		{
			StartChasing();
		}
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

bool AEnemy::InTargetRange(TObjectPtr<AActor> Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DisToTarget = (Target->GetActorLocation() - this->GetActorLocation()).Size();
	return DisToTarget <= Radius;
}

void AEnemy::MoveToTarget(TObjectPtr<AActor> Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	EnemyController->MoveTo(MoveRequest);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, FollowRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(CurrentPatrolTarget, PatrolRadius))
	{
		CurrentPatrolTarget = ChoosePatrolTarget();
		const float surveyTime = FMath::RandRange(WaitTimeMin, WaitTimeMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, surveyTime);
	}
}

TObjectPtr<AActor> AEnemy::ChoosePatrolTarget()
{
	TArray<TObjectPtr<AActor>> ValidTargets;
	for (TObjectPtr<AActor>Target : PatrolTargets)
	{
		if (Target != CurrentPatrolTarget)
			ValidTargets.AddUnique(Target);
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 randPatrolTarget = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[randPatrolTarget];
	}
	return nullptr;
}

void AEnemy::StartPartrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(CurrentPatrolTarget);
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(CurrentPatrolTarget);
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::StartChasing()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	ToggleHealthBarVisibility(false);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::IsPatrolling()
{
	return EnemyState == EEnemyState::EES_Patrolling;
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

void AEnemy::ToggleHealthBarVisibility(bool isVisible)
{
	if (HealthBar)
	{
		HealthBar->SetVisibility(isVisible);
	}
}





