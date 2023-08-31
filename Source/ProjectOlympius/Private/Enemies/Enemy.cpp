#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "ProjectOlympius/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Enemies/Enemy.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute Component"));

	HealthBar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar"));
	HealthBar->SetupAttachment(GetRootComponent());
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBar)
	{
		HealthBar->SetHealthPercent(1.0f);
		HealthBar->SetVisibility(false);
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

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector toHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(forward, toHit);
	double theta = FMath::Acos(CosTheta);
	theta = FMath::RadiansToDegrees(theta);
	const FVector crossProduct = FVector::CrossProduct(forward, toHit);

	if (crossProduct.Z < 0)
	{
		theta *= -1.0f;
	}

	FName section("ReactBack");
	if (theta >= -45.0f && theta < 45.0f)
	{
		section = FName("ReactFront");
	}
	else if (theta >= -135.0f && theta < -45.0f)
	{
		section = FName("ReactLeft");
	}
	else if (theta >= 45.0f && theta < 135.0f)
	{
		section = FName("ReactRight");
	}

	PlayOnHitMontage(FName(section));

	//Debugging
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Purple, FString::Printf(TEXT("Theta: %f"), theta));
	//}
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + crossProduct * 100.0f, 5.0f, FColor::Green, 5.0f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + forward * 60.0f, 5.0f, FColor::Green, 5.0f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + toHit * 60.0f, 5.0f, FColor::Orange, 5.0f);
}

void AEnemy::PlayOnHitMontage(const FName& SectionName)
{
	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && OnHitMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hello"));
		AnimInstance->Montage_Play(OnHitMontage);
		AnimInstance->Montage_JumpToSection(SectionName, OnHitMontage);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		const double DisToTarget = (Target->GetActorLocation() - this->GetActorLocation()).Size();
		if (DisToTarget > FollowRadius)
		{
			Target = nullptr;
			if (HealthBar)
			{
				HealthBar->SetVisibility(false);
			}
		}
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
	Target = EventInstigator->GetPawn();
	return DamageAmount;
}

