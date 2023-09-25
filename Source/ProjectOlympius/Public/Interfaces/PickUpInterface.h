/*
Author(s): Kody Wood
Description: Interface that Pick up items will have to dictate when they are picked up
*/
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpInterface.generated.h"

//Forward Declaration
class AItem;
class ASoul;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickUpInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class PROJECTOLYMPIUS_API IPickUpInterface
{
	GENERATED_BODY()
public:
	virtual void SetOverlappingItem(TObjectPtr<AItem> Item);
	virtual void AddSouls(TObjectPtr<ASoul> Soul);
};
