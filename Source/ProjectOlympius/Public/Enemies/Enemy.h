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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInistigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

protected:

	UPROPERTY(EditAnywhere, Category = Combat)
		float DeathTimeSpan = 5.0f;

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
		EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	virtual void BeginPlay() override;
	virtual void OnDeath() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void CheckCombatTarget();
	void CheckPatrolTarget();
	bool InTargetRange(TObjectPtr<AActor> Target, double Radius);
	void MoveToTarget(TObjectPtr<AActor> Target);
	TObjectPtr<AActor> ChoosePatrolTarget();

	UFUNCTION()
		void PawnSeen(APawn* SeenPawn);

	virtual bool CanAttack() override;
	virtual void Attack() override;

	virtual int32 PlayDeathMontage() override;

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

	//--- AI Behaviour ---//
	void ToggleHealthBarVisibility(bool isVisible);
	void LoseInterest();
	void StartPartrolling();
	void StartChasing(); 
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsPatrolling();
	bool IsChasing();
	bool IsAttacking();
	bool IsEngaged();
	bool IsDead();
	void ClearPatrolTimer();


	UPROPERTY(EditAnywhere, Category = "Combat")
		float PatrollingSpeed = 125.0f;
	UPROPERTY(EditAnywhere)
		float ChasingSpeed = 300.0f;

	//---Combat Behavious ---//

	void StartAttackTimer();
	void ClearAttackTimer();
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.0f;

	UPROPERTY()
		TObjectPtr<AAIController> EnemyController;
};
