#include "MainGameMode.h"
#include "Nobody.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
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
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->CreateWidgetInstance();
	}
	
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		PlayerCharacter->SetInputEnable(true);
	}
	
	EventSpawnManager->SpawnEvent(CurrentStageNum);
	
	GetWorldTimerManager().SetTimer(EventSpawnTimerHandle, this, &ThisClass::StartStage, UMathLibrary::GetRandomInRange(EventSpawnDelay), false);
}

void AMainGameMode::StopStage()
{
	GetWorldTimerManager().ClearTimer(EventSpawnTimerHandle);
	EventSpawnManager->StopEvent();
	
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->RemoveWidgetInstance();
	}
}

void AMainGameMode::RestartStage()
{
	StopStage();
	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this, true)));
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
}


