/*
Author(s): Kody Wood
Description: Component Script that will handle the processes of the health bar
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

//--- Forward Declarations ---//
class UHealthBar;
UCLASS()
class PROJECTOLYMPIUS_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);
private:
	UPROPERTY()
	TObjectPtr<UHealthBar> HealthBarWidget;
};
