/*
Author(s): Kody Wood
Description: Class derived from Item.h class that controls Soul pick up items
*/
#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

//Forward Declarations
class UNiagaraSystem;
UCLASS()
class PROJECTOLYMPIUS_API ASoul : public AItem
{
	GENERATED_BODY()
	
public:
	//--------- Public Functions ---------//
	virtual void Tick(float DeltaTime) override;

	//--------- Public Variables ---------//
	FORCEINLINE int32 GetSoulWorth() const { return SoulWorth; }
	FORCEINLINE void SetSoulWorth(int32 Value) { SoulWorth = Value; }
protected:
	//--------- Protected Functions ---------//
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//--------- Protected Variables ---------//
private:
	//--------- Private Functions ---------//

	//--------- Private Variables ---------//
	UPROPERTY(EditAnywhere, Category = " Soul Properties")
		int32 SoulWorth;

	double DesiredZ;

	UPROPERTY(EditAnywhere)
	float DriftRate = -15.0f;


};