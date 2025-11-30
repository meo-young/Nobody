#include "PlayerCharacter.h"
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Nobody.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/InteractionComponent.h"

APlayerCharacter::APlayerCharacter()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Move(TEXT("/Game/_Nobody/Input/IA_Move"));
	if (IA_Move.Succeeded())
	{
		MoveAction = IA_Move.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Look(TEXT("/Game/_Nobody/Input/IA_Look"));
	if (IA_Move.Succeeded())
	{
		LookAction = IA_Look.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Interact(TEXT("/Game/_Nobody/Input/IA_Interact"));
	if (IA_Move.Succeeded())
	{
		InteractAction = IA_Interact.Object;
	}
	
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = 100.0f;
	CharacterMovementComponent->MaxStepHeight = 15.0f;
	
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(RootComponent);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::DoMove);
		}
		
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::DoLook);
		}
		
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::DoInteract);
		}
	}
}

void APlayerCharacter::DoMove(const FInputActionValue& InputActionValue)
{
	const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
	
	AddMovementInput(GetActorForwardVector(), MoveValue.X);
	AddMovementInput(GetActorRightVector(), MoveValue.Y);
}

void APlayerCharacter::DoLook(const FInputActionValue& InputActionValue)
{
	const FVector2D LookValue = InputActionValue.Get<FVector2D>();
	
	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}

void APlayerCharacter::DoInteract(const FInputActionValue& InputActionValue)
{
	InteractionComponent->ExecuteInteractIfPossible();
}
