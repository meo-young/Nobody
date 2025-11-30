#include "Interaction/EmptyDoor.h"
#include "Enum/EInteractType.h"

AEmptyDoor::AEmptyDoor()
{
	InteractionType = EInteractionType::Inspect;
}

void AEmptyDoor::Interact_Implementation()
{
	Super::Interact_Implementation();
}
