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
#include "GameMode/MainGameMode.h"
#include "Library/SoundLibrary.h"
#include "Manager/ScareSoundManager.h"
#include "PlayerController/PlayerControllerBase.h"
#include "UI/Manual/ManualWidget.h"
#include "Sound/SoundCue.h"

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
	
	if (AMainGameMode* GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (UScareSoundManager* ScareSoundManager = GameMode->GetScareSoundManager())
		{
			ScareSoundManager->AddDoor(this);
		}
	}
}

bool AOpenableDoor::HasPlayScarySound()
{
	if (!bIsInteractPossible)
	{
		return false;
	}
	
	if (ScarySounds.IsEmpty())
	{
		return false;
	}
	
	// 랜덤으로 무서운 소리를 재생한 후 배열에서 제외합니다.
	uint8 RandomIndex = FMath::RandRange(0, ScarySounds.Num() - 1);
	USoundCue* RandomSound = ScarySounds[RandomIndex];
	USoundLibrary::PlaySFXInLocation(this, RandomSound, ScarySoundLocation);
	
	ScarySounds.RemoveAt(RandomIndex);
	
	LOG(TEXT("Scare Sound 재생합니다"))
	
	return true;
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
		USoundLibrary::PlaySFXInLocation(GetWorld(), ESFX::Flash_Hum, GetActorLocation());
		SpotLightComponent->SetVisibility(false);
	}
	
}

void AOpenableDoor::InitEvent()
{
	if (GetIsEventActivated())
	{
		USoundLibrary::PlaySFXInLocation(GetWorld(), ESFX::Flash_On, GetActorLocation());
		SpotLightComponent->SetVisibility(true);	
	}
	
	Super::InitEvent();
	
	PlayerController->GetManualWidget()->SetRightClickAction(TEXT("물러서기"));
	PlayerController->GetManualWidget()->ShowWidget();
}

void AOpenableDoor::OnStartActorSequenceEnded()
{
	Super::OnStartActorSequenceEnded();
	
	// 손전등을 활성화합니다.
	SpotLightComponent->SetVisibility(true);
	USoundLibrary::PlaySFXInLocation(GetWorld(), ESFX::Flash_On, GetActorLocation());
}
