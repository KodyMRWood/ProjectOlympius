/*
Author(s): Kody Wood
Description: Functionalty for the item actor
*/

#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Pawns/Bird.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	m_Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	m_Capsule->SetCapsuleHalfHeight(20.0f);
	m_Capsule->SetCapsuleRadius(15.0f);
	SetRootComponent(m_Capsule);

	m_BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	m_BirdMesh->SetupAttachment(GetRootComponent());

	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_SpringArm->SetupAttachment(GetRootComponent());
	m_SpringArm->TargetArmLength = 300.0f;

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	m_Camera->SetupAttachment(m_SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
}

void ABird::Move(const FInputActionValue& value)
{
	const float dirValue= value.Get<float>();
	if (Controller && dirValue != 0.0f)
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, dirValue);
	}
}

void ABird::Look(const FInputActionValue& value)
{
	const FVector2D lookAxisValue = value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(lookAxisValue.X);
		AddControllerPitchInput(lookAxisValue.Y);
	}
}

//Old Input System Functions
/*
void ABird::MoveForward(float value)
{
	if (Controller && value != 0.0f)
	{
		FVector Forward = GetActorForwardVector();
		UE_LOG(LogTemp, Warning, TEXT("MoveForward"));
		AddMovementInput(Forward, value);
	}
}

void ABird::Turn(float value)
{
	AddControllerYawInput(value);
}

void ABird::Look(float value)
{
	AddControllerPitchInput(value);
}
*/

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
	}
		
	//Old Way
	//PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABird::MoveForward);
	//PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::Turn);
	//PlayerInputComponent->BindAxis(FName("Look"), this, &ABird::Look);
}

