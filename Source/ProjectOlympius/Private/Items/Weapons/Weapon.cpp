// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/OlympiusCharacter.h"


void AWeapon::Equip(TObjectPtr<USceneComponent> InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIs_Equipped;
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation()
		);
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	RootItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent,OtherActor, OtherComp, OtherBodyIndex);
}
