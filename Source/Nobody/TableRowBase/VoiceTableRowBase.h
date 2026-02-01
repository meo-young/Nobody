#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "VoiceTableRowBase.generated.h"

enum class EVoice : uint8;

USTRUCT()
struct FVoiceTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EVoice Voice;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundCue> Sound;
	
	UPROPERTY(EditAnywhere)
	FString DialogueText;
	
};