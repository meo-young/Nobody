#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScareSoundManager.generated.h"

class AOpenableDoor;

UCLASS()
class NOBODY_API UScareSoundManager : public UObject
{
	GENERATED_BODY()
	
public:
	void StartSpawnSoundTimer();
	void StopSpawnSound();
	void SpawnSound();
	void AddDoor(const TObjectPtr<AOpenableDoor> Door);
	
private:
	/** 타이머를 활성화하는 함수입니다. */
	void ActivateTimer();
	
private:
	UPROPERTY()
	TArray<TObjectPtr<AOpenableDoor>> Doors;
	
	FTimerHandle ScareSoundTimerHandle;
	
	float ScareSoundInterval = 120.0f;
	
};
