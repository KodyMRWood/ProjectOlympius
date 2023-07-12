/*
Author(s): Kody Wood
Description: Functionalty for the item actor
*/

#include "Items/Item.h"
#include "UObject/ObjectPtr.h"
#include "ProjectOlympius/DebugMacros.h"

AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	SetRootComponent(RootItemMesh);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

float AItem::TransformedSin()
{
	return m_Amplitude * FMath::Sin(m_RunningTime * m_TimeConstant);
}
float AItem::TransformedCos()
{
	return m_Amplitude * FMath::Cos(m_RunningTime * m_TimeConstant);
}


void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_RunningTime += DeltaTime;

}

