#include "Interaction/Bed.h"
#include "Character/Player/PlayerCharacter.h"
#include "Enum/EInteractType.h"
#include "PlayerController/PlayerControllerBase.h"
#include "UI/Manual/ManualWidget.h"

ABed::ABed()
{
	InteractionType = EInteractionType::Hide;
	
	BedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BedMesh"));
	BedMesh->SetupAttachment(Root);
	
	PillowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillowMesh"));
	PillowMesh->SetupAttachment(BedMesh);
}

void ABed::BeginPlay()
{
	Super::BeginPlay();
}

void ABed::Interact_Implementation()
{
	Super::Interact_Implementation();

	Player->SetEffectEnable(false);
}

void ABed::OnEndActorSequenceEnded()
{
	Super::OnEndActorSequenceEnded();

	Player->SetEffectEnable(true);
}

void ABed::InitEvent()
{
	Super::InitEvent();
	
	PlayerController->GetManualWidget()->SetRightClickAction(TEXT("물러서기"));
	PlayerController->GetManualWidget()->ShowWidget();
}
