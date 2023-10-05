/*
Author(s): Kody Wood
Description: Base parent class for all character like entities in the game (i.e. Main Character, Enemy)
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "BaseCharacter.generated.h"


//Forward Declarations
class UAnimeMontage;
class AWeapon;
class UAttributeComponent;

UCLASS()
class PROJECTOLYMPIUS_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()
public:
	//--------- Public Functions ---------//
	ABaseCharacter();
	//<AActor>
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }

	//--------- Public Variables ---------//

protected:
	//--------- Protected Functions ---------//
	//<AActor>
	virtual void BeginPlay() override;

	//--- Combat ---//
	virtual void GetHit_Implementation(const FVector& ImpactPoint, const AActor* Hitter) override;
	virtual void Attack();
	virtual bool CanAttack();
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd();
	virtual void HandleDamage(float DamageAmount);
	bool IsAlive();
	UFUNCTION(BlueprintNativeEvent)
	void OnDeath();

	void DisableCapsule();
	UFUNCTION(BlueprintCallable)
		void ToggleWeaponCollision(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
		FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
		FVector GetRotationWarpTarget();

	//--- Montage/Animation ---//
	virtual int32 PlayAttackMontage();
	virtual void PlayDodgeMontage();
	void StopAttackMontage();
	void PlayOnHitMontage(const FName& SectionName);
	virtual void DirectionalHitReact(const FVector& ImpactPoint);
	virtual int32 PlayDeathMontage();

	//--- Sound FX  ---//
	void PlayHitSound(const FVector& ImpactPoint);

	//--- Particle FX ---//
	void PlayHitParticles(const FVector& ImpactPoint);

	//--------- Protected Variables ---------//
	//--- Components ---//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UAttributeComponent> Attributes;

	//--- Equipment ---//
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		TObjectPtr<AWeapon> EquippedWeapon;

	//--- Combat ---//
	UPROPERTY(BlueprintReadWrite, Category = Combat)
		TObjectPtr<AActor> CombatTarget;
	UPROPERTY(EditAnywhere, Category = Combat)
		double WarpTargetDistance = 75.0f;

	//--- Montage/Animation ---//
	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;

private:

	//--------- Private Functions ---------//
	//--- Montage/Animation ---//
	void PlayMontageSection(TObjectPtr<UAnimMontage> AnimMontage, const FName& SectionName);
	int32 PlayRandomMontageSection(TObjectPtr<UAnimMontage> AnimMontage, const TArray<FName>& SectionNames);

	//--------- Private Variables ---------//
	//--- Montage/Animation ---//
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		TObjectPtr<UAnimMontage> DodgeMontage;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		TObjectPtr<UAnimMontage> OnHitMontage;
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		TObjectPtr<UAnimMontage> DeathMontage;
	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> AttackMontageSections;
	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> DeathMontageSections;

	//--- Sound FX ---//
	UPROPERTY(EditAnywhere, Category = Combat)
		TObjectPtr<USoundBase> HitSound;

	//--- Particle FX ---//
	UPROPERTY(EditAnywhere, Category = Combat)
		TObjectPtr<UParticleSystem> HitParticles;
};