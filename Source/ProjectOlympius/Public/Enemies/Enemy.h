#pragma once
/*
Author(s): Kody Wood
Description: Parent class for all enemy types
*/

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
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

	//--------- Public Functions ---------//
	AEnemy();

	//AActor
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInistigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

	//HitInterface
	virtual void GetHit_Implementation(const FVector& ImpactPoint, const AActor* Hitter) override;

	//--------- Public Variables ---------//


protected:

	//--------- Protected Functions ---------//
	//<AActor>
	virtual void BeginPlay() override;

	//<ABaseCharacter>
	virtual bool CanAttack() override;
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void OnDeath() override;

	//--------- Protected Variables ---------//
	UPROPERTY(BlueprintReadWrite)
		EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:

	//--------- Private Functions ---------//
	//--- Init ---//
	void InitEnemy();
	void SpawnDefaultWeapon();
	void SpawnInitialPatrolTarget();

	//--- AI Behaviour ---//
	//Navigation
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent
	void CheckCombatTarget();
	bool InTargetRange(TObjectPtr<AActor> Target, double Radius);
	void MoveToTarget(TObjectPtr<AActor> Target);
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	//Patrolling
	void CheckPatrolTarget();
	TObjectPtr<AActor> ChoosePatrolTarget();
	void StartPartrolling();
	void PatrolTimerFinished();
	void ClearPatrolTimer();
	//Chasing
	void StartChasing();
	void LoseInterest();
	//Attacking
	void StartAttackTimer();
	void ClearAttackTimer();
	//StateChecks
	bool IsPatrolling();
	bool IsChasing();
	bool IsAttacking();
	bool IsEngaged();
	bool IsDead();
	
	//--- Health Bar ---//
	void ToggleHealthBarVisibility(bool isVisible);

	//--------- Private Variables---------//
	//--- Components ---//
	UPROPERTY()
		TObjectPtr<AAIController> EnemyController;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UHealthBarComponent> HealthBar;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UPawnSensingComponent> PawnSensor;
	UPROPERTY(EditAnywhere)
		TSubclassOf<AWeapon> WeaponClass;

	//--- AI Behaviour ---//
	//UPROPERTY()
	//	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere)
		float ChasingSpeed = 300.0f;
	UPROPERTY(EditAnywhere)
		double FollowRadius = 500.0f;
	UPROPERTY(EditAnywhere)
		double AttackRadius = 150.0f;
	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.0f;

	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float AttackMax = 1.0f;
	UPROPERTY(EditAnywhere, Category = Combat)
		float DeathTimeSpan = 5.0f;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TObjectPtr<AActor> CurrentPatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<TObjectPtr<AActor>> PatrolTargets;
	FTimerHandle PatrolTimer;
	UPROPERTY(EditAnywhere)
		float WaitTimeMin = 5.0f;
	UPROPERTY(EditAnywhere)
		float WaitTimeMax = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float PatrollingSpeed = 125.0f;
};
