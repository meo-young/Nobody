#pragma once

#include "CoreMinimal.h"
#include "GameMode/BaseGameMode.h"
#include "MainGameMode.generated.h"

class UEventSpawnManager;

UCLASS()
class NOBODY_API AMainGameMode : public ABaseGameMode
{
	GENERATED_BODY()
	
public:
	AMainGameMode();
	
public:
	virtual void PostInitProperties() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	virtual void InitGameState() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
private:
	UPROPERTY()
	TObjectPtr<UEventSpawnManager> EventSpawnManager;
	
private:
	uint8 CurrentStageNum = 0;
	
public:
	FORCEINLINE UEventSpawnManager* GetEventSpawnManager() const { return EventSpawnManager; }
	
};
