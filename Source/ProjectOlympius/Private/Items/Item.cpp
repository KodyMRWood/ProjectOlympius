/*
Author(s): Kody Wood
Description: Functionalty for the item actor
*/

#include "Items/Item.h"
#include "NiagaraComponent.h"
#include "UObject/ObjectPtr.h"
#include "Components/SphereComponent.h"
#include "ProjectOlympius/DebugMacros.h"
#include "Characters/OlympiusCharacter.h"

AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
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
	TObjectPtr<AOlympiusCharacter> OlympiusCharacter = Cast<AOlympiusCharacter>(OtherActor);
	if (OlympiusCharacter)
	{
		OlympiusCharacter->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TObjectPtr<AOlympiusCharacter> OlympiusCharacter = Cast<AOlympiusCharacter>(OtherActor);
	if (OlympiusCharacter)
	{
		OlympiusCharacter->SetOverlappingItem(nullptr);
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

