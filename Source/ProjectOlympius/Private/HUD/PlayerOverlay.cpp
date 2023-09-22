#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HUD/PlayerOverlay.h"

void UPlayerOverlay::SetHealthBarPercent(float percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(percent);
	}
}

void UPlayerOverlay::SetStaminaBarPercent(float percent)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(percent);
	}
}

void UPlayerOverlay::SetGold(int32 gold)
{
	if (GoldText)
	{	
		GoldText->SetText(FText::FromString(FString::Printf(TEXT("%d"), gold)));
	}
}

void UPlayerOverlay::SetSouls(int32 souls)
{
	if (SoulsText)
	{
		SoulsText->SetText(FText::FromString(FString::Printf(TEXT("%d"), souls)));
	}
}
