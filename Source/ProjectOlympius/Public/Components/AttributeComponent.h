/*
Author(s): Kody Wood
Description: Component that will hold the attributes for the main character or other entities (enemies)
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTOLYMPIUS_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ReceiveDamage(float Damage);
	float GetHealthPercent();
	bool IsAlive();

	void AddGold(int32 GoldAmount);
	void AddSouls(int32 SoulsAmount);


	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float CurrentHealth;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float MaxHealth;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 Gold;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 Souls;

};
