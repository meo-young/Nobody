#include "Manager/EventSpawnManager.h"

#include "EngineUtils.h"
#include "LevelSequencePlayer.h"
#include "Nobody.h"
#include "Character/Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"

void UEventSpawnManager::SpawnEvent(const uint8 InCurrentLevel)
{
	if (Enemies.IsEmpty())
	{
		LOG(TEXT("소환할 이벤트가 없습니다."))
		return;
	}
		
	const uint8 RandomNumber = FMath::RandRange(0, Enemies.Num()-1);
	
	Enemies[RandomNumber]->StartStepSystem();
	Enemies.RemoveAt(RandomNumber);
	
	LOG(TEXT("이벤트가 스폰되었습니다."))
}

void UEventSpawnManager::AddEnemy(const TObjectPtr<AEnemyBase> InEnemy)
{
	Enemies.Add(InEnemy);
	
	LOG(TEXT("Enemy를 추가합니다"))
}

void UEventSpawnManager::StopEvent()
{
	for (AEnemyBase* Enemy : TActorRange<AEnemyBase>(GetWorld()))
	{
		Enemy->StopStepSystem();
	}
}

