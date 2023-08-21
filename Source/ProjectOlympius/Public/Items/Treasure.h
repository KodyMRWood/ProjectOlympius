/*
Author(s): Kody Wood
Description: Parent class for all treasure actors types
*/
#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTOLYMPIUS_API ATreasure : public AItem
{
	GENERATED_BODY()
public:
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
	UPROPERTY(EditAnywhere, Category = Sounds)
		TObjectPtr<USoundBase> PickupSound;

	UPROPERTY(EditAnywhere, category = "Treasure Properties")
		uint32 GoldAmount;
};
