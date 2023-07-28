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
	virtual void GetHit(const FVector& ImpactPoint) override;

	void PlayOnHitMontage(const FName& SectionName);
private:	

	//--- Animation Montages ---//
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> OnHitMontage;


};
