// Fill out your copyright notice in the Description page of Project Settings.

#include "Interfaces/PickUpInterface.h"
#include "Items/Soul.h"

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		PickUpInterface->AddSouls(this);
		SpawnPickedUpEffect();
		SpawnPickedUpSound();
		Destroy();
	}

}
