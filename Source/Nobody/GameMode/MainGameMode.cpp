#include "MainGameMode.h"

#include "Nobody.h"
#include "Character/Player/PlayerCharacter.h"
#include "Manager/EventSpawnManager.h"
#include "PlayerController/PlayerControllerBase.h"

AMainGameMode::AMainGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerCharacter> BP_Player(TEXT("/Game/_Nobody/Blueprint/Character/Player/BP_Player"));
	if (BP_Player.Succeeded())
	{
		DefaultPawnClass = BP_Player.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerControllerBase> BP_PlayerController(TEXT("/Game/_Nobody/Blueprint/PlayerController/BP_MainPlayerController"));
	if (BP_PlayerController.Succeeded())
	{
		PlayerControllerClass = BP_PlayerController.Class;
	}
}

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
	
	EventSpawnManager->SpawnEvent(CurrentStageNum);
}


