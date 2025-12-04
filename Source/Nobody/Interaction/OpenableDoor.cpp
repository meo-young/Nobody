#include "Interaction/OpenableDoor.h"

#include "EnhancedInputComponent.h"
#include "Nobody.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SpotLightComponent.h"
#include "Enum/EInteractType.h"
#include "GameFramework/SpringArmComponent.h"
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

void AOpenableDoor::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	// 액터 시퀀스를 재생합니다.
	PlayActorSequence();

	// 액터 시퀀스가 재생하는 동안 플레이어 캐릭터를  숨깁니다.
	Player->SetActorHiddenInGame(true);
	
	// 플레이어 카메라를 SequenceCameraComponent로 전환합니다.
	if (PlayerController.Get())
	{
		PlayerController->SetViewTargetWithBlend(
			this,                   
			0.5f,                   
			VTBlend_Cubic
		);
	}
}

void AOpenableDoor::OnActorSequenceEnded()
{
	Super::OnActorSequenceEnded();
	
	// 손전등을 활성화합니다.
	SpotLightComponent->SetVisibility(true);
}