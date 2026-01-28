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
	virtual void PostInitProperties() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	virtual void InitGameState() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
protected:
	/** 현재 스테이지를 시작하는 함수입니다. */
	void StartStage();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float EventSpawnDelay = 20.0f;
	
private:
	UPROPERTY()
	TObjectPtr<UEventSpawnManager> EventSpawnManager;
	
private:
	uint8 CurrentStageNum = 0;
	FTimerHandle EventSpawnTimerHandle;
	
public:
	FORCEINLINE UEventSpawnManager* GetEventSpawnManager() const { return EventSpawnManager; }
	
};
