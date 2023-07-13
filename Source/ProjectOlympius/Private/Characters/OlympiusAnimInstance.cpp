/*
Author(s): Kody Wood
Description: Definitions for Animation Instance
*/

#include "Characters/OlympiusCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/OlympiusAnimInstance.h"

void UOlympiusAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	MainCharacter = Cast<AOlympiusCharacter>(TryGetPawnOwner());
	if (MainCharacter)
	{
		MainCharacterMovement = MainCharacter->GetCharacterMovement();
	}
}
	

void UOlympiusAnimInstance::NativeUpdateAnimation(float deltaTime)
{
	Super::NativeUpdateAnimation(deltaTime);

	if (MainCharacterMovement)
	{
		groundSpeed = UKismetMathLibrary::VSizeXY(MainCharacterMovement->Velocity);
		isFalling = MainCharacterMovement->IsFalling();
	}
}
