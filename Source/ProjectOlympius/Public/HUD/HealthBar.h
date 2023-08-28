/*
Author(s): Kody Wood
Description: Script to handle the health bar variables
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

//--- Forward Declarations ---//
class UProgressBar;

UCLASS()
class PROJECTOLYMPIUS_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
};
