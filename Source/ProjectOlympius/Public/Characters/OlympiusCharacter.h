/*
Author(s): Kody Wood
Description: Functionalty for the main character
*/
#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "InputActionValue.h"
#include "OlympiusCharacter.generated.h"

//Forward Declarations
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UAnimMontage;
class AItem;

UCLASS()
class PROJECTOLYMPIUS_API AOlympiusCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AOlympiusCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

protected:
	virtual void BeginPlay() override;

	//--- Inputs ---//
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> InputContext;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MovementAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> DodgeAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> EPressedAction;
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	virtual void Jump() override;
	virtual void Attack() override;
	void Dodge();
	void EPressed();

	//--- Montage Functions ---//
	void PlayEquipMontage(const FName &SectionName);

	
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	bool CanUnequip();
	bool CanEquip();
	UFUNCTION(BlueprintCallable)
	void UnequipItem();
	UFUNCTION(BlueprintCallable)
	void EquipItem();
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

private:
	//Spring Arm and Camera
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USpringArmComponent> CameraArm;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UCameraComponent> ViewCamera;

	//Hair and Eyebrows
	UPROPERTY(VisibleAnywhere, Category = Hair)
		TObjectPtr<UGroomComponent> Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
		TObjectPtr<UGroomComponent> Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;

	//--- Character States (Animations) ---//
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	//--- Animation Montages ---//
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> EquipMontage;


public:
	FORCEINLINE void SetOverlappingItem(TObjectPtr<AItem> Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState()  const { return CharacterState; }
};
