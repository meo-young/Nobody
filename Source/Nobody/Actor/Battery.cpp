#include "Actor/Battery.h"
#include "Nobody.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Define/Define.h"
#include "Enum/EInteractType.h"
#include "Enum/ESFX.h"
#include "Kismet/GameplayStatics.h"
#include "Library/SoundLibrary.h"

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
	
	USoundLibrary::PlaySFXInLocation(this, ESFX::Battery_Get, GetActorLocation());

	// 빙의 상태와 무관하게 PlayerCharacter 액터를 직접 찾아 배터리를 충전합니다.
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass())))
	{
		LOG(TEXT("배터리 획득 - 손전등 충전 완료"));
		Player->RechargeBattery();
	}

	//@TODO: 배터리 획득하는 소리
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
