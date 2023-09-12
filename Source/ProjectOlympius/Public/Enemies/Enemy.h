#pragma once
/*
Author(s): Kody Wood
Description: Parent class for all enemy types
*/

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"


class AAIController;
class UHealthBarComponent;
class UPawnSensingComponent;
class AWeapon;

UCLASS()
class PROJECTOLYMPIUS_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:

	AEnemy();

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInistigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

protected:
	//UPROPERTY(BlueprintReadOnly)
	UPROPERTY(VisibleAnywhere)
		EDeathPose DeathPose = EDeathPose::EDP_Alive;

	virtual void BeginPlay() override;
	virtual void OnDeath() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	

	void CheckCombatTarget();
	void CheckPatrolTarget();
	bool InTargetRange(TObjectPtr<AActor> Target, double Radius);
	void MoveToTarget(TObjectPtr<AActor> Target);
	TObjectPtr<AActor> ChoosePatrolTarget();

	UFUNCTION()
		void PawnSeen(APawn* SeenPawn);

private:
	//--- Components ---//
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UHealthBarComponent> HealthBar;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UPawnSensingComponent> PawnSensor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;

	//--- Combat Variables ---//
	UPROPERTY()
		TObjectPtr<AActor> CombatTarget;
	UPROPERTY(EditAnywhere)
		double FollowRadius = 500.0f;
	UPROPERTY(EditAnywhere)
		double AttackRadius = 150.0f;
	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.0f;

	//--- Navigation ---//
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<TObjectPtr<AActor>> PatrolTargets;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere)
	float WaitTimeMin = 5.0f;
	UPROPERTY(EditAnywhere)
	float WaitTimeMax = 10.0f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY()
	TObjectPtr<AAIController> EnemyController;
};
