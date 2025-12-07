#include "Interaction/Bed.h"
#include "Enum/EInteractType.h"

ABed::ABed()
{
	InteractionType = EInteractionType::Hide;
	
	BedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BedMesh"));
	BedMesh->SetupAttachment(Root);
	
	PillowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillowMesh"));
	PillowMesh->SetupAttachment(BedMesh);
}
