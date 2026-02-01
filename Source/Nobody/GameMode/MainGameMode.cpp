#include "MainGameMode.h"
#include "Nobody.h"
#include "Character/Player/PlayerCharacter.h"
#include "Library/MathLibrary.h"
#include "Manager/EventSpawnManager.h"
#include "Manager/ScareSoundManager.h"
#include "PlayerController/PlayerControllerBase.h"

AMainGameMode::AMainGameMode()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = APlayerControllerBase::StaticClass();
}

void AMainGameMode::PostInitProperties()
{
	Super::PostInitProperties(); 	
}

void AMainGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
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
	
	CurrentStageNum = 0;
	
	EventSpawnManager = NewObject<UEventSpawnManager>(this);
	ScareSoundManager = NewObject<UScareSoundManager>(this);
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainGameMode::StartPlay()
{
	Super::StartPlay();
	
	ScareSoundManager->StartSpawnSoundTimer();
	GetWorldTimerManager().SetTimer(EventSpawnTimerHandle, this, &ThisClass::StartStage, UMathLibrary::GetRandomInRange(EventSpawnDelay), false);
}


