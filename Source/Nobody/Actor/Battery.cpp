#include "Actor/Battery.h"
#include "Nobody.h"
#include "Components/BoxComponent.h"
#include "Define/Define.h"
#include "Enum/EInteractType.h"

ABattery::ABattery()
{
	BatteryMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatteryMesh"));
	BatteryMesh->SetupAttachment(RootComponent);
	
	InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZone"));
	InteractionZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionZone->SetCollisionResponseToChannel(ECC_INTERACTION, ECR_Block);
	InteractionZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	InteractionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InteractionZone->SetupAttachment(BatteryMesh);
	
	InteractionType = EInteractionType::Inspect;
}

void ABattery::Interact_Implementation()
{
	IInteractable::Interact_Implementation();
	
	//@TODO: 배터리 획득하는 소리
	LOG(TEXT("상호작용"));
	Destroy();
}

EInteractionType ABattery::GetInteractionType()
{
	return InteractionType;
}

void ABattery::EnableInteraction()
{
	InteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABattery::DisableInteraction()
{
	InteractionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
