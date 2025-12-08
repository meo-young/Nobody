#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "BlurWidget.generated.h"

class UBackgroundBlur;

UCLASS()
class NOBODY_API UBlurWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	void SetBlurIntensity(float Intensity);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBackgroundBlur> Blur;
	
};
