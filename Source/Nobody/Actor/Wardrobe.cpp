#include "Actor/Wardrobe.h"

#include "Nobody.h"
#include "Character/Player/PlayerCharacter.h"
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

	OriginalLocation = WardrobeDoorHandleMesh->GetComponentLocation();

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		bWasHiding = Player->GetIsHiding();
		ScheduleNextStep();
	}
}

void AWardrobe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player) return;

	// 숨기 상태 변화를 감지합니다.
	const bool bCurrentlyHiding = Player->GetIsHiding();
	if (bCurrentlyHiding != bWasHiding)
	{
		bWasHiding = bCurrentlyHiding;

		// 진행 중인 단계 타이머를 취소합니다.
		GetWorldTimerManager().ClearTimer(StageTimerHandle);

		// 이동 중이 아닌 경우 즉시 다음 스텝을 스케줄합니다.
		// 이동 중이라면 이동 완료 후 ScheduleNextStep이 호출됩니다.
		if (!bIsMoving)
		{
			ScheduleNextStep();
		}
	}

	// 문을 목표 위치로 보간 이동합니다.
	if (bIsMoving)
	{
		const FVector Target = GetStageLocation(CurrentStage);
		const FVector Current = WardrobeDoorHandleMesh->GetComponentLocation();

		if (Current.Equals(Target, 0.5f))
		{
			WardrobeDoorHandleMesh->SetWorldLocation(Target);
			bIsMoving = false;
			ScheduleNextStep();
		}
		else
		{
			const FVector NewLocation = FMath::VInterpConstantTo(Current, Target, DeltaTime, MoveSpeed);
			WardrobeDoorHandleMesh->SetWorldLocation(NewLocation);
		}
	}
}

FVector AWardrobe::GetStageLocation(int32 Stage) const
{
	if (Stage <= 0) return OriginalLocation;

	float TotalOffset = 0.f;
	for (int32 i = 0; i < Stage && i < StageOffsets.Num(); i++)
	{
		TotalOffset += StageOffsets[i];
	}
	return OriginalLocation + FVector(0.f, TotalOffset, 0.f);
}

void AWardrobe::ScheduleNextStep()
{
	if (!Player) return;

	const bool bIsHiding = Player->GetIsHiding();

	if (bIsHiding)
	{
		// 숨은 상태: 단계가 남아있으면 역단계 타이머를 스케줄합니다.
		if (CurrentStage > 0)
		{
			GetWorldTimerManager().SetTimer(
				StageTimerHandle, this, &AWardrobe::RetreatStage, ClosingStageDelay, false);
		}
		else
		{
			LOG(TEXT("모두 닫힘"));
		}
	}
	else
	{
		// 노출 상태: 최대 단계 미만이면 다음 열기 타이머를 스케줄합니다.
		if (CurrentStage < MaxStage)
		{
			const float Delay = OpeningDelays.IsValidIndex(CurrentStage)
				? OpeningDelays[CurrentStage] : 1.f;
			GetWorldTimerManager().SetTimer(
				StageTimerHandle, this, &AWardrobe::AdvanceStage, Delay, false);
		}
		else
		{
			LOG(TEXT("모두 열림"));
		}
	}
}

void AWardrobe::AdvanceStage()
{
	if (CurrentStage >= MaxStage) return;

	CurrentStage++;
	bIsMoving = true;
}

void AWardrobe::RetreatStage()
{
	if (CurrentStage <= 0) return;

	CurrentStage--;
	bIsMoving = true;
}