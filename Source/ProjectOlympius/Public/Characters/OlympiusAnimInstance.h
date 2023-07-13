#pragma once
/*
Author(s): Kody Wood
Description: Handles the animations for the main character
*/

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OlympiusAnimInstance.generated.h"

// Forward Declaration
class AOlympiusCharacter;
class UCharacterMovementComponent;

UCLASS()
class PROJECTOLYMPIUS_API UOlympiusAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float deltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AOlympiusCharacter> MainCharacter;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TObjectPtr<UCharacterMovementComponent> MainCharacterMovement;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float groundSpeed;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool isFalling;
};