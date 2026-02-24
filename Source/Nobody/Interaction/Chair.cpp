#include "Interaction/Chair.h"
#include "Enum/EInteractType.h"
#include "PlayerController/PlayerControllerBase.h"
#include "UI/Manual/ManualWidget.h"

AChair::AChair()
{
	InteractionType = EInteractionType::Hide;
	
	ChairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMesh"));
	ChairMesh->SetupAttachment(Root);
}

void AChair::InitEvent()
{
	Super::InitEvent();
	
	PlayerController->GetManualWidget()->SetLeftClickAction(TEXT("재생하기"));
	PlayerController->GetManualWidget()->SetRightClickAction(TEXT("물러서기"));
	PlayerController->GetManualWidget()->ShowWidget();
}
