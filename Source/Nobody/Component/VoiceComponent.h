#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VoiceComponent.generated.h"

enum class EVoiceType : uint8;
enum class EVoice : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOBODY_API UVoiceComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	void PlayVoiceWithDelay(EVoiceType InVoiceType, const float InDelay = 0.0f);
	
private:
	void PlayVoice();
	void ResetVoiceTimer();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	TArray<EVoice> EmptyDoorVoices;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float VoiceCooldownTime = 60.0f;
	
private:
	FTimerHandle VoiceTimerHandle;
	EVoiceType CurrentVoiceType;
	
	uint8 bIsEnableVoice : 1 = true;

		
};
