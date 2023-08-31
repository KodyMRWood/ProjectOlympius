#pragma once
/*
Author(s): Kody Wood
Description: Parent class for all enemy types
*/

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UAnimeMontage;
class UAttributeComponent;
class UHealthBarComponent;

UCLASS()
class PROJECTOLYMPIUS_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()
public:

	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInistigator, AActor* DamageCauser) override;
protected:
	//UPROPERTY(BlueprintReadOnly)
	UPROPERTY(VisibleAnywhere)
		EDeathPose DeathPose = EDeathPose::EDP_Alive;

	virtual void BeginPlay() override;
	void OnDeath();
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayOnHitMontage(const FName& SectionName);

private:
	//--- Components ---//
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UAttributeComponent> Attributes;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UHealthBarComponent> HealthBar;

	//--- Animation Montages ---//
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		TObjectPtr<UAnimMontage> OnHitMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
		TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
		TObjectPtr<UParticleSystem> HitParticles;

	//--- Combat Variables ---//
	UPROPERTY()
		TObjectPtr<AActor> Target;
	UPROPERTY(EditAnywhere)
		double FollowRadius = 500.0f;
};
