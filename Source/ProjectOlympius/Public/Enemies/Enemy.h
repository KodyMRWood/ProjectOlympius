#pragma once
/*
Author(s): Kody Wood
Description: Parent class for all enemy types
*/

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"	
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
protected:
	virtual void BeginPlay() override;
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

	UPROPERTY(EditAnywhere, Category = Sounds)
		TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
		TObjectPtr<UParticleSystem> HitParticles;
};
