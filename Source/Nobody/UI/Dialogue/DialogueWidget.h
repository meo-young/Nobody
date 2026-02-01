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
	void SetText(const FString& Text);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogueText;
	
};
