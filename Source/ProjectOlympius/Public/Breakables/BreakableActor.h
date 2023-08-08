#pragma once
/*
Author(s): Kody Wood
Description: Parent class for all breakable actors types
*/


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

//--- Forward Declaractions ---//
class UGeometryCollectionComponent;
UCLASS()
class PROJECTOLYMPIUS_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

};
