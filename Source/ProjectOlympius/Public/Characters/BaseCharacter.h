/*
Author(s): Kody Wood
Description: Base parent class for all character like entities in the game (i.e. Main Character, Enemy)
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"


//Forward Declarations
//class UInputAction;
//class UInputMappingContext;
//class USpringArmComponent;
//class UCameraComponent;
//class UGroomComponent;
//class UAnimMontage;
//class AItem;
class UAnimeMontage;
class AWeapon;
class UAttributeComponent;

UCLASS()
class PROJECTOLYMPIUS_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()
public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void ToggleWeaponCollision(ECollisionEnabled::Type CollisionEnabled);
protected:
	virtual void BeginPlay() override;

	bool IsAlive();
	virtual void HandleDamage(float DamageAmount);

	//--- Attack ---//
	virtual void Attack();

	//--- Death ---//
	virtual void OnDeath();

	//--- Components ---//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UAttributeComponent> Attributes;

	//--- Equipment ---//
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		TObjectPtr<AWeapon> EquippedWeapon;

	//--- Montage/Animation Variables ---//
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		TObjectPtr<UAnimMontage> OnHitMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		TObjectPtr<UAnimMontage> DeathMontage;

	//--- Montage/Animation Functions ---//
	virtual void PlayAttackMontage();
	void PlayOnHitMontage(const FName& SectionName);
	virtual void DirectionalHitReact(const FVector& ImpactPoint);

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	virtual bool CanAttack();

	//--- Sound FX  ---//
	void PlayHitSound(const FVector& ImpactPoint);

	//--- Particle FX ---//
	void PlayHitParticles(const FVector& ImpactPoint);

private:	
	//--- Sound FX ---//
	UPROPERTY(EditAnywhere, Category = Sounds)
		TObjectPtr<USoundBase> HitSound;

	//--- Particle FX ---//
	UPROPERTY(EditAnywhere, Category = VisualEffects)
		TObjectPtr<UParticleSystem> HitParticles;
};