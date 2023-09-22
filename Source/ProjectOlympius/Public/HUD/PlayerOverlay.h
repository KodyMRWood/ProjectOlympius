/*
Author(s): Kody Wood
Description: Controls the HUD for the player character
*/
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

//Forward Declarations
class UProgressBar;
class UTextBlock;

UCLASS()
class PROJECTOLYMPIUS_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	//--------- Public Functions ---------//
	void SetHealthBarPercent(float percent);
	void SetStaminaBarPercent(float percent);
	void SetGold(int32 gold);
	void SetSouls(int32 souls);

	//--------- Public Variables ---------//

protected:
	//--------- Protected Functions ---------//
				
				
	//--------- Protected Variables ---------//
private:
	//--------- Private Functions ---------//
				
				
	//--------- Private Variables ---------//
	UPROPERTY(meta = (BindWidget));
	TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(meta = (BindWidget));
	TObjectPtr<UProgressBar> StaminaBar;
	UPROPERTY(meta = (BindWidget));
	TObjectPtr<UTextBlock> GoldText;
	UPROPERTY(meta = (BindWidget));
	TObjectPtr<UTextBlock> SoulsText;
};
