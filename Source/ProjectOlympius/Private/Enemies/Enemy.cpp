#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
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


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBar)
	{
		HealthBar->SetHealthPercent(1.0f);
		HealthBar->SetVisibility(false);
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
}

void AEnemy::OnDeath()
{
	if (HealthBar)
	{
		HealthBar->SetVisibility(false);
	}

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

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{

	if (HealthBar)
	{
		HealthBar->SetVisibility(true);
	}

	if (Attributes && Attributes->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		OnDeath();
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
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

	//if (Target == nullptr)
	//{
	//	const int32 randPatrolTarget = FMath::RandRange(0, PatrolTargets.Num() - 1);
	//	TObjectPtr<AActor> newTarget = PatrolTargets[randPatrolTarget];
	//	Target = newTarget;
	//}

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.0f);
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
	if (EnemyState == EEnemyState::EES_Chasing) return;
	if (SeenPawn->ActorHasTag(FName("Player")))
	{
		
		EnemyState = EEnemyState::EES_Chasing;
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		CombatTarget = SeenPawn;

		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
		}
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(CurrentPatrolTarget);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}


void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, FollowRadius))
	{
		CombatTarget = nullptr;
		if (HealthBar)
		{
			HealthBar->SetVisibility(false);
		}
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.0f;
		MoveToTarget(CurrentPatrolTarget);
	}
	else if(!InTargetRange(CombatTarget,AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		MoveToTarget(CombatTarget);
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		EnemyState = EEnemyState::EES_Attacking;
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

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBar)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBar->SetHealthPercent(Attributes->GetHealthPercent());
	}
	CombatTarget = EventInstigator->GetPawn();
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	MoveToTarget(CombatTarget); 
	return DamageAmount;
}

