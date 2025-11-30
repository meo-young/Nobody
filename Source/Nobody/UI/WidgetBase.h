#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

UCLASS()
class NOBODY_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void ShowWidget();
	
	UFUNCTION()
	void HideWidget();
	
};
