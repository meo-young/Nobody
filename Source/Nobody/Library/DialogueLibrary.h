#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DialogueLibrary.generated.h"

enum class EVoice : uint8;
class UDialogueSubsystem;

UCLASS()
class NOBODY_API UDialogueLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** 특정 음성 대사를 화면에 표시하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	static void ShowDialogue(const UObject* WorldContextObject, EVoice InVoice);
	static void ShowDialogue(const UObject* WorldContextObject, EVoice InVoice, const FVector& InLocation);
	
private:
	/** 월드 컨텍스트 객체로부터 사운드 서브시스템을 가져오는 헬퍼 함수입니다. */
	static UDialogueSubsystem* GetDialogueSubsystem(const UObject* WorldContextObject);
	
};
