/*
Author(s): Kody Wood
Description: CPP for the main character
*/

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"


#include "Characters/OlympiusCharacter.h"

// Sets default values
AOlympiusCharacter::AOlympiusCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->TargetArmLength = 300.0f;
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("Head");
	
	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("Head");
}

void AOlympiusCharacter::Tick(float DeltaTime)
{
}

void AOlympiusCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputContext, 0);
		}
	}
}

void AOlympiusCharacter::Move(const FInputActionValue& value)
{
	if (ActionState == EActionState::EAS_Attacking) return;

	const FVector2D movementVector = value.Get<FVector2D>();
	const FRotator rotation = Controller->GetControlRotation();
	const FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);
	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(forwardDirection, movementVector.Y);
	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(rightDirection, movementVector.X);
	//ActionState = EActionState::EAS_Moving;
}

void AOlympiusCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookAxisVector = value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AOlympiusCharacter::Jump()
{
	Super::Jump();
	//ActionState = EActionState::EAS_Jumping;
}

void AOlympiusCharacter::Attack()
{
	if(CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool AOlympiusCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		   CharacterState != ECharacterState::ECS_Unequipped;
}

void AOlympiusCharacter::Dodge()
{
	UE_LOG(LogTemp, Warning, TEXT("Dodge"));
}

void AOlympiusCharacter::EPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("E Pressed"));
	TObjectPtr<AWeapon> OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	}
}

void AOlympiusCharacter::PlayAttackMontage()
{
	TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 selection = FMath::RandRange(0, 4);
		FName SectionName = FName();
		switch (selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		case 3:
			SectionName = FName("Attack4");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AOlympiusCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AOlympiusCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AOlympiusCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AOlympiusCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AOlympiusCharacter::Jump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AOlympiusCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &AOlympiusCharacter::Dodge);
		EnhancedInputComponent->BindAction(EPressedAction, ETriggerEvent::Triggered, this, &AOlympiusCharacter::EPressed);
	}
}

