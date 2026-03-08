#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Clock.generated.h"

class ULevelSequencePlayer;
class ULevelSequence;
class USoundCue;

UCLASS()
class NOBODY_API AClock : public AActor
{
	GENERATED_BODY()
	
public:	
	AClock();
	virtual void BeginPlay() override;
	
public:
	void ActivateTimer();
	void PauseTimer();
	
private:
	void CountTime();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> ClockMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "변수|컴포넌트")
	TObjectPtr<UAudioComponent> AudioComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|사운드")
	TObjectPtr<USoundCue> ClockSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	uint8 TimeInterval = 3;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|시퀀스")
	TObjectPtr<ULevelSequence> TimeUpSequence;
	
	UPROPERTY(BlueprintReadOnly)
	FString CurrentTimeAsString;
	
private:
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> TimeUpSequenceActor;
	
	FTimerHandle TimerHandle;
	int32 CurrentTime = 0;

};
