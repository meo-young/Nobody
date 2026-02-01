#include "Manager/ScareSoundManager.h"
#include "Nobody.h"
#include "Interaction/OpenableDoor.h"
#include "Library/MathLibrary.h"

void UScareSoundManager::StartSpawnSoundTimer()
{
	ActivateTimer();
}

void UScareSoundManager::StopSpawnSound()
{
	// 타이머를 중지합니다.
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(ScareSoundTimerHandle);
	}
}

void UScareSoundManager::SpawnSound()
{
	// 유효한 문이 없는 경우 타이머를 재활성화합니다.
	if (Doors.Num() == 0)
	{
		LOG(TEXT("유효한 문이 없습니다"))
		ActivateTimer();
		return;
	}

	// 재생 가능한 문들의 인덱스를 저장할 배열입니다.
	TArray<int32> AvailableDoorIndices;

	// 문 배열의 복사본을 생성하여 셔플합니다.
	TArray<int32> ShuffledIndices;
	for (int32 i = 0; i < Doors.Num(); ++i)
	{
		ShuffledIndices.Add(i);
	}

	// 인덱스를 무작위로 섞습니다.
	for (int32 i = ShuffledIndices.Num() - 1; i > 0; --i)
	{
		int32 j = FMath::RandRange(0, i);
		ShuffledIndices.Swap(i, j);
	}

	// 셔플된 순서대로 문들을 확인합니다.
	for (int32 Index : ShuffledIndices)
	{
		if (Doors.IsValidIndex(Index) && Doors[Index])
		{
			// HasPlayScarySound가 true를 반환하면 재생 성공입니다.
			if (Doors[Index]->HasPlayScarySound())
			{
				// 타이머를 재활성화합니다.
				ActivateTimer();
				return;
			}
		}
	}

	// 모든 문이 false를 반환한 경우 타이머를 재활성화합니다.
	LOG(TEXT("모든 문이 false를 반환합니다."))
	ActivateTimer();
}

void UScareSoundManager::AddDoor(const TObjectPtr<AOpenableDoor> Door)
{
	Doors.Add(Door);
	LOG(TEXT("%s 문을 추가합니다."), *Door->GetName());
}

void UScareSoundManager::ActivateTimer()
{
	// 월드를 가져옵니다.
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 기존 타이머를 정리합니다.
	World->GetTimerManager().ClearTimer(ScareSoundTimerHandle);

	// 120초 후 SpawnSound를 호출하는 타이머를 설정합니다.
	World->GetTimerManager().SetTimer(
		ScareSoundTimerHandle,
		this,
		&UScareSoundManager::SpawnSound,
		UMathLibrary::GetRandomInRange(ScareSoundInterval),
		false
	);
	
	LOG(TEXT("타이머 활성화"))
}
