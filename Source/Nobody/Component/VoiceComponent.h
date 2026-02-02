#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VoiceComponent.generated.h"

enum class EVoice : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOBODY_API UVoiceComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	EVoice VoiceType;

		
};
