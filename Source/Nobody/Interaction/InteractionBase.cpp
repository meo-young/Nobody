#include "Interaction/InteractionBase.h"
#include "Nobody.h"
#include "Components/ShapeComponent.h"
#include "Define/Define.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerBase.h"
#include "UI/Crosshair/CrosshairWidget.h"

AInteractionBase::AInteractionBase()
{
	GetCollisionComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCollisionComponent()->SetCollisionResponseToChannel(ECC_INTERACTION, ECR_Block);
	GetCollisionComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
}

void AInteractionBase::Interact_Implementation()
{
	IInteractable::Interact_Implementation();
	
	LOG(TEXT("상호작용을 수행합니다."));
}
