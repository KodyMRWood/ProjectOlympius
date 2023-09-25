// Fill out your copyright notice in the Description page of Project Settings.
/*
Author(s): Kody Wood
Description: Class that will help to determine if something was hit and by what
*/
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTOLYMPIUS_API IHitInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void GetHit(const FVector& ImpactPoint, const AActor* Hitter);
};
