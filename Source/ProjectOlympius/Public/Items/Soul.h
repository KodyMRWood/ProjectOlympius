/*
Author(s): Kody Wood
Description: Class derived from Item.h class that controls Soul pick up items
*/
#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

//Forward Declarations

UCLASS()
class PROJECTOLYMPIUS_API ASoul : public AItem
{
	GENERATED_BODY()
	
public:
	//--------- Public Functions ---------//


	//--------- Public Variables ---------//

protected:
	//--------- Protected Functions ---------//
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//--------- Protected Variables ---------//
private:
	//--------- Private Functions ---------//

	//--------- Private Variables ---------//

};
