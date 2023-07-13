/*
Author(s): Kody Wood
Description: Functionalty for the main character
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "OlympiusCharacter.generated.h"

//Forward Declarations
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;

UCLASS()
class PROJECTOLYMPIUS_API AOlympiusCharacter : public ACharacter
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
	void Attack();
	void Dodge();
	void EPressed();


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
};
