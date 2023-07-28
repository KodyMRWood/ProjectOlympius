#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "ProjectOlympius/DebugMacros.h"
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
	//DRAW_DEBUG_SPHERE_COLOR(ImpactPoint, FColor::Red);
	PlayOnHitMontage(FName("ReactFront"));
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

