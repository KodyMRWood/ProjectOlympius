/*
Author(s): Kody Wood
Description: Functionalty for the item actor
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

//Forward Declarations
class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

enum class EItemState :uint8
{
	EIS_Hovering,
	EIs_Equipped
};

UCLASS()
class PROJECTOLYMPIUS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float m_Amplitude = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float m_TimeConstant = 0.5;

	UFUNCTION(BlueprintPure)
	float TransformedSin();
	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template<typename T>
	T Average(T first, T second);

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	virtual void SpawnPickedUpEffect();
	virtual void SpawnPickedUpSound();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> RootItemMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	//--- Item States ---//
	EItemState ItemState = EItemState::EIS_Hovering;

	//--- Particle System ---//
	UPROPERTY(EditAnywhere)
		TObjectPtr<UNiagaraComponent> PickUpParticles;
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* PickedUpEffect;

	//--- Sound Effects ---//
	UPROPERTY(EditAnywhere)
		USoundBase* PickedUpSound;
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float m_RunningTime = 0.0f;

};


template <typename T>
inline T AItem::Average(T first, T second)
{
	return (first + second) / 2;
}
