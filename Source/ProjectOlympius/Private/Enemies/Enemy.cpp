#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HUD/HealthBarComponent.h"
#include "AIController.h"
#include "Enemies/Enemy.h"
#include "Components/AttributeComponent.h"

#include "ProjectOlympius/DebugMacros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


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
	StartChasing();
	return DamageAmount;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBar)
	{
		HealthBar->SetHealthPercent(1.0f);
		ToggleHealthBarVisibility(false);
	}

	EnemyController = Cast<AAIController>(GetController());
	if (CurrentPatrolTarget == nullptr)
	{
		CurrentPatrolTarget = ChoosePatrolTarget();
	}
	MoveToTarget(CurrentPatrolTarget);

	if (PawnSensor)
	{
		PawnSensor->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;

	}
}

void AEnemy::OnDeath()
{
	ToggleHealthBarVisibility(false);

	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		const int32 selection = FMath::RandRange(0, 2);
		FName SectionName = FName();
		switch (selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(3.0f);
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBar)
	{
		HealthBar->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	ToggleHealthBarVisibility(true);

	if (IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		OnDeath();
	}
	PlayHitSound(ImpactPoint);
	PlayHitParticles(ImpactPoint);
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

	//if (Target == nullptr)
	//{
	//	const int32 randPatrolTarget = FMath::RandRange(0, PatrolTargets.Num() - 1);
	//	TObjectPtr<AActor> newTarget = PatrolTargets[randPatrolTarget];
	//	Target = newTarget;
	//}

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(45.0f);
	EnemyController->MoveTo(MoveRequest);
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

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("Player"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		StartChasing();
	}
}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsDead();
	return bCanAttack;
}

void AEnemy::Attack()
{
	Super::Attack();
	PlayAttackMontage();
}

void AEnemy::PlayAttackMontage()
{
	Super::PlayAttackMontage();
	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 selection = FMath::RandRange(0, 3);
		FName SectionName = FName();
		switch (selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(CurrentPatrolTarget);
}

void AEnemy::ToggleHealthBarVisibility(bool isVisible)
{
	if (HealthBar)
	{
		HealthBar->SetVisibility(isVisible);
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	ToggleHealthBarVisibility(false);
}

void AEnemy::StartPartrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(CurrentPatrolTarget);
}

void AEnemy::StartChasing()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, FollowRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsPatrolling()
{
	return EnemyState == EEnemyState::EES_Patrolling;
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

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
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

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
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
	else if (IsInsideAttackRadius() && !IsAttacking())
	{
		StartAttackTimer();
	}
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

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

