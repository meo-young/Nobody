#include "Interaction/Bed.h"

#include "Nobody.h"
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
	// 플레이어가 침대의 오른쪽에서 상호작용했는지 판별합니다.
	const FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
	bIsRightSide = FVector::DotProduct(ToPlayer, GetActorRightVector()) > 0.f;
	
	LOG(TEXT(" Player %s"), bIsRightSide ? TEXT("오른쪽") : TEXT("왼쪽"));

	Super::Interact_Implementation();

	Player->SetEffectEnable(false);
	Player->SetIsHiding(true);
}

void ABed::OnEndActorSequenceEnded()
{
	Super::OnEndActorSequenceEnded();

	Player->SetEffectEnable(true);
	Player->SetIsHiding(false);
}

void ABed::InitEvent()
{
	Super::InitEvent();
	
	PlayerController->GetManualWidget()->SetRightClickAction(TEXT("물러서기"));
	PlayerController->GetManualWidget()->ShowWidget();
}
