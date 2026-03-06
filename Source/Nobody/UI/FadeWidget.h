#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "FadeWidget.generated.h"

class UImage;

UCLASS()
class NOBODY_API UFadeWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> FadeImage;
	
};
