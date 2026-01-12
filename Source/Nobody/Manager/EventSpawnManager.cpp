#include "Manager/EventSpawnManager.h"
#include "LevelSequencePlayer.h"
#include "Nobody.h"
#include "Character/Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"

void UEventSpawnManager::Init()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), FoundActors);
	
	for (AActor* Actor : FoundActors)
	{
		if (AEnemyBase* Enemy = Cast<AEnemyBase>(Actor))
		{
			Enemies.Add(Enemy);
		}
	}
}

void UEventSpawnManager::SpawnEvent(const uint8 InCurrentLevel)
{
	if (Enemies.IsEmpty()) return;
	
	const uint8 RandomNumber = FMath::RandRange(0, InCurrentLevel);
	
	Enemies[RandomNumber]->StartStepSystem();
	Enemies.RemoveAt(RandomNumber);
	
	LOG(TEXT("이벤트가 스폰되었습니다."))
}

void UEventSpawnManager::AddEnemy(const TObjectPtr<AEnemyBase> InEnemy)
{
	Enemies.Add(InEnemy);
	
	LOG(TEXT("Enemy를 추가합니다"))
}

