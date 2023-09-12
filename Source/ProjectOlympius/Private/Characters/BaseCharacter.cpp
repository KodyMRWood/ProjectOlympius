
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/AttributeComponent.h"
#include "Characters/BaseCharacter.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attribute Component"));
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::ToggleWeaponCollision(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);

	}
	EquippedWeapon->IgnoreActors.Empty();
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::OnDeath()
{
}

void ABaseCharacter::PlayAttackMontage()
{
}

void ABaseCharacter::PlayOnHitMontage(const FName& SectionName)
{
	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && OnHitMontage)
	{
		AnimInstance->Montage_Play(OnHitMontage);
		AnimInstance->Montage_JumpToSection(SectionName, OnHitMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
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
}

void ABaseCharacter::AttackEnd()
{
}

bool ABaseCharacter::CanAttack()
{
	return false;
}



