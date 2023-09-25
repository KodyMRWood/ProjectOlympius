/*
Author(s): Kody Wood
Description: Functionalty for the item actor
*/

#include "Items/Item.h"
#include "NiagaraComponent.h"
#include "UObject/ObjectPtr.h"
#include "Components/SphereComponent.h"
#include "ProjectOlympius/DebugMacros.h"
#include "Interfaces/PickUpInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RootItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(RootItemMesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	PickUpParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Pick Up Particles"));
	PickUpParticles->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin()
{
	return m_Amplitude * FMath::Sin(m_RunningTime * m_TimeConstant);
}
float AItem::TransformedCos()
{
	return m_Amplitude * FMath::Cos(m_RunningTime * m_TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlapperComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		PickUpInterface->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		PickUpInterface->SetOverlappingItem(nullptr);
	}
}

void AItem::SpawnPickedUpEffect()
{
	if (PickedUpEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickedUpEffect,
			GetActorLocation()
		);
	}
}

void AItem::SpawnPickedUpSound()
{
	if (PickedUpSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickedUpSound,
			GetActorLocation()
		);
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.0f, 0.0f, TransformedSin()));
	}
}

