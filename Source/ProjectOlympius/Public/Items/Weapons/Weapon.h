#pragma once
/*
Author(s): Kody Wood
Description: Functionalty for the Weapon actor, Child of Item
*/

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECTOLYMPIUS_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	void Equip(TObjectPtr<USceneComponent> InParent, FName InSocketName);
	void AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName& InSocketName);
protected:
	virtual void BeginPlay() override;
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override; 
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, Category ="Weapon Properties")
	TObjectPtr<USoundBase> EquipSound;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<UBoxComponent> WeaponBox;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USceneComponent> BoxTraceStart;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USceneComponent> BoxTraceEnd;
};
