#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth-Damage,0.0f, MaxHealth);
}

float UAttributeComponent::GetHealthPercent()
{
	return CurrentHealth/ MaxHealth;
}

