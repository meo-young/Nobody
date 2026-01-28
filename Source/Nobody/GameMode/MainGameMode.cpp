#include "MainGameMode.h"
#include "Nobody.h"
#include "Character/Player/PlayerCharacter.h"
#include "Library/MathLibrary.h"
#include "Manager/EventSpawnManager.h"
#include "PlayerController/PlayerControllerBase.h"

void AMainGameMode::PostInitProperties()
{
	Super::PostInitProperties(); 	
	
	CurrentStageNum = 0;
}

void AMainGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	EventSpawnManager = NewObject<UEventSpawnManager>(this);
	EventSpawnManager->Init();
}

void AMainGameMode::StartStage()
{
	EventSpawnManager->SpawnEvent(CurrentStageNum);
	
	GetWorldTimerManager().SetTimer(EventSpawnTimerHandle, this, &ThisClass::StartStage, UMathLibrary::GetRandomInRange(EventSpawnDelay), false);
}

void AMainGameMode::InitGameState()
{
	Super::InitGameState();
}

void AMainGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainGameMode::StartPlay()
{
	Super::StartPlay();
	
	GetWorldTimerManager().SetTimer(EventSpawnTimerHandle, this, &ThisClass::StartStage, UMathLibrary::GetRandomInRange(EventSpawnDelay), false);
}


