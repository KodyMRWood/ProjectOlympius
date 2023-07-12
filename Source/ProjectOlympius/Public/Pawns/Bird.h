#pragma once

/*
Author(s): Kody Wood
Description: Functionalty for the item actor
*/
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Bird.generated.h"

//Forward Declarations
class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class PROJECTOLYMPIUS_API ABird : public APawn
{
	GENERATED_BODY()
public:
	ABird();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> BirdMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	//Old Input System
	//void MoveForward(float value);
	//void Turn(float value);
	//void Look(float value);


private:
	//--- Components ---//
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> m_Capsule;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> m_BirdMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> m_SpringArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> m_Camera;

};
