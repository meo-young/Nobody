#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "TitleWidget.generated.h"

class UTextBlock;

UCLASS()
class NOBODY_API UTitleWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	void SetText(const FString& NewTitle);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;
	
};
