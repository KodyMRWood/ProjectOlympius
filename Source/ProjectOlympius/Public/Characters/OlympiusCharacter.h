/*
Author(s): Kody Wood
Description: Functionalty for the main character
*/
#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Interfaces/PickUpInterface.h"
#include "InputActionValue.h"
#include "OlympiusCharacter.generated.h"

//Forward Declarations
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UAnimMontage;
class AItem;
class ASoul;
class UPlayerOverlay;

UCLASS()
class PROJECTOLYMPIUS_API AOlympiusCharacter : public ABaseCharacter, public IPickUpInterface
{
	GENERATED_BODY()

public:
	//--------- Public Functions ---------//
	AOlympiusCharacter();

	//<AActor>
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInistigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//<IHitInterface>
	virtual void GetHit_Implementation(const FVector& ImpactPoint, const AActor* Hitter) override;

	//<IPickUpInterface>
	virtual void SetOverlappingItem(TObjectPtr<AItem>  Item) override;
	virtual void AddSouls(TObjectPtr<ASoul>  Soul) override;

	FORCEINLINE ECharacterState GetCharacterState()  const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }


	//--------- Public Variables---------//

protected:
	//--------- Protected Functions ---------//
	//<AActor>
	virtual void BeginPlay() override;

	//--- Inputs ---//
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	virtual void Jump() override;
	virtual void Attack() override;
	void Dodge();
	void EPressed();

	//--- Combat ---//
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	bool CanEquip();
	void PickUpWeapon(TObjectPtr<AWeapon> Weapon);
	void Equip();
	bool CanUnequip();
	void Unequip();
	UFUNCTION(BlueprintCallable)
		void AttachWeaponToBack();
	UFUNCTION(BlueprintCallable)
		void AttachWeaponToHand();
	UFUNCTION(BlueprintCallable)
		void FinishEquipping();
	UFUNCTION(BlueprintCallable)
		void HitReactEnd();
	virtual void OnDeath() override;


	//--- Montage ---//
	void PlayEquipMontage(const FName& SectionName);

	//--------- Protected Variables ---------//
	//--- Inputs ---//
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> InputContext;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MovementAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> DodgeAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> EPressedAction;

private:
	//--------- Private Functions ---------//
	void InitOverlay();
	void InitInputSystem();
	void SetHUDHealth();
	bool IsUnoccupied();

	//--------- Private Variables ---------//
	//--- Controllers ---//
	TObjectPtr<APlayerController> PlayerController;

	//--- HUD ---//
	UPROPERTY();
	TObjectPtr<UPlayerOverlay> PlayerOverlay;

	//--- Components ---//
	//Spring Arm and Camera
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USpringArmComponent> CameraArm;
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UCameraComponent> ViewCamera;

	//Hair and Eyebrows
	UPROPERTY(VisibleAnywhere, Category = Hair)
		TObjectPtr<UGroomComponent> Hair;
	UPROPERTY(VisibleAnywhere, Category = Hair)
		TObjectPtr<UGroomComponent> Eyebrows;

	//--- Interactions ---//
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;

	//--- Character States---//
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	//--- Animation Montages ---//
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> EquipMontage;

};
