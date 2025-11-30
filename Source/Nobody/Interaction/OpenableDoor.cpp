#include "Interaction/OpenableDoor.h"
#include "Components/BoxComponent.h"
#include "Components/ShapeComponent.h"
#include "Enum/EInteractType.h"

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
}

void AOpenableDoor::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	// 액터 시퀀스를 재생합니다.
	PlayActorSequence();
	
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
