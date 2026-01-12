#include "Interaction/OpenableDoor.h"

#include "EnhancedInputComponent.h"
#include "Nobody.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SpotLightComponent.h"
#include "Enum/EInteractType.h"
#include "Enum/ESFX.h"
#include "GameFramework/SpringArmComponent.h"
#include "Library/SoundLibrary.h"
#include "Pawn/SpotlightCamera.h"

AOpenableDoor::AOpenableDoor()
{
	InteractionType = EInteractionType::Inspect;
	
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());
	
	Doorknob = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Doorknob"));
	Doorknob->SetupAttachment(Door);
	
	InteractionZone->SetupAttachment(Door);
	InteractionZone->SetRelativeLocation(FVector(7.0f, 91.5f, 99.5f));
	InteractionZone->SetBoxExtent(FVector(5.0f, 10.0f, 10.0f));

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CameraComponent);

	SpotLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SpotLightComponent->SetupAttachment(SpringArmComponent);
	SpotLightComponent->SetLightingChannels(false, true, false);
	SpotLightComponent->SetVisibility(false);
}

void AOpenableDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AOpenableDoor::DoControl(const FInputActionValue& Value)
{
	if (!bIsInteractPossible) return;
	
	// 손전등을 활성화합니다.
	SpotLightComponent->SetVisibility(false);
	USoundLibrary::PlaySFXInLocation(GetWorld(), ESFX::Flash_Off, GetActorLocation());

	Super::DoControl(Value);
}

void AOpenableDoor::CheckIfEventActivated()
{
	Super::CheckIfEventActivated();
	
	if (GetIsEventActivated())
	{
		// @TODO : 지지직 하는 소리 추가 필요
		SpotLightComponent->SetVisibility(false);
	}
	
}

void AOpenableDoor::InitEvent()
{
	// @TODO : 지지직 하는 소리 추가 필요
	SpotLightComponent->SetVisibility(true);
	
	Super::InitEvent();
}

void AOpenableDoor::OnStartActorSequenceEnded()
{
	Super::OnStartActorSequenceEnded();
	
	// 손전등을 활성화합니다.
	SpotLightComponent->SetVisibility(true);
	USoundLibrary::PlaySFXInLocation(GetWorld(), ESFX::Flash_On, GetActorLocation());
}
