#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "DialogueWidget.generated.h"

class UTextBlock;

UCLASS()
class NOBODY_API UDialogueWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetText(const FString& Text, const float DisplayDuration);
	
private:
	/** 다이얼로그 텍스트를 표시하는 TextBlock입니다. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueText;
    
	/** 다이얼로그를 자동으로 숨기기 위한 타이머 핸들입니다. */
	FTimerHandle HideTimerHandle;
	
};
