// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OlympiusCharacter.h"
#include "Interfaces/PickUpInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Treasure.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		PickUpInterface->AddGold(this);
		SpawnPickedUpSound();
		Destroy();
	}

}
