// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/OlympiusCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Treasure.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<AOlympiusCharacter> OlympiusCharacter = Cast<AOlympiusCharacter>(OtherActor);
	if (OlympiusCharacter)
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSound,
				GetActorLocation()
			);
		}
		Destroy();
	}
}
