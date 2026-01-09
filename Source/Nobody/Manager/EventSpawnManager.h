#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EventSpawnManager.generated.h"

class AEnemyBase;

UCLASS(Blueprintable)
class NOBODY_API UEventSpawnManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Init();
	void SpawnEvent(const uint8 InCurrentLevel);
	void AddEnemy(const TObjectPtr<AEnemyBase> InEnemy);
	
protected:
	TArray<TObjectPtr<AEnemyBase>> Enemies;
	
};
