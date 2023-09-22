/*
Author(s): Kody Wood
Description: HUD class to communicate from class to HUD, Spawns Widget
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

//Forward Declarations
class UPlayerOverlay;

UCLASS()
class PROJECTOLYMPIUS_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
public:
	//--------- Public Functions ---------//

	//--------- Public Variables ---------//

protected:
	//--------- Protected Functions ---------//
	virtual void BeginPlay() override;

	//--------- Protected Variables ---------//
private:
	//--------- Private Functions ---------//

	//--------- Private Variables ---------//
	UPROPERTY(EditDefaultsOnly, Category = Player)
		TSubclassOf<UPlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
	TObjectPtr<UPlayerOverlay> PlayerOverlay;
	
};
