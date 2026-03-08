#include "Actor/Wardrobe.h"

#include "Nobody.h"
#include "Character/Player/PlayerCharacter.h"
#include "Interaction/Bed.h"
#include "Kismet/GameplayStatics.h"

AWardrobe::AWardrobe()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	WardrobeDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WardrobeDoor Mesh"));
	WardrobeDoorMesh->SetupAttachment(Root);

	WardrobeDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WardrobeDrawer Mesh"));
	WardrobeDrawerMesh->SetupAttachment(Root);

	WardrobeBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WardrobeBody Mesh"));
	WardrobeBodyMesh->SetupAttachment(Root);

	WardrobeDoorHandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WardrobeDoorHandle Mesh"));
	WardrobeDoorHandleMesh->SetupAttachment(Root);

	PrimaryActorTick.bCanEverTick = true;
}

void AWardrobe::BeginPlay()
{
	Super::BeginPlay();
	
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AWardrobe::StartEvent()
{
	bEventActive = true;
	bEventCleared = false;
	HidingAccumulatedTime = 0.0f;

	PlayEventSequence();
}

void AWardrobe::OnEventSequenceEnded()
{
	if (!bEventActive) return;

	bEventActive = false;

	if (bEventCleared)
	{
		LOG(TEXT("이벤트 클리어"));
		StopEventSequence();
		return;
	}

	LOG(TEXT("이벤트 실패 - DeadSequence 재생"));
	if (Player)
	{
		Player->ExecuteDeathSequence(JumpScareType);
	}
}

void AWardrobe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player) return;

	// 이벤트 진행 중이면 Bed 은신 시간을 누적합니다.
	if (bEventActive && !bEventCleared)
	{
		if (Player->GetIsHiding())
		{
			HidingAccumulatedTime += DeltaTime;
			if (HidingAccumulatedTime >= RequiredHidingDuration)
			{
				bEventCleared = true;
				OnEventSequenceEnded();
			}
		}
		else
		{
			// 은신 해제 시 누적 시간 초기화 (연속 2초 요구)
			HidingAccumulatedTime = 0.0f;
		}
	}
}