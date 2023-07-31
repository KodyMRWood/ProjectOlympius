#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "ProjectOlympius/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Enemies/Enemy.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DirectionalHitReact(ImpactPoint);
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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Purple, FString::Printf(TEXT("Theta: %f"), theta));
	}
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + crossProduct * 100.0f, 5.0f, FColor::Green, 5.0f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + forward * 60.0f, 5.0f, FColor::Green, 5.0f);
	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + toHit * 60.0f, 5.0f, FColor::Orange, 5.0f);
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

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

