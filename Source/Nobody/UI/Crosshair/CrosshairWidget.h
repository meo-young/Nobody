#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "CrosshairWidget.generated.h"

class UImage;
enum class EInteractionType : uint8;

UCLASS()
class NOBODY_API UCrosshairWidget : public UWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	
public:
	void SetCrosshair(EInteractionType InteractionType);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Crosshair_Image;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|텍스처")
	TObjectPtr<UTexture2D> NoneTexture;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|텍스처")
	TObjectPtr<UTexture2D> InspectTexture;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|텍스처")
	TObjectPtr<UTexture2D> HideTexture;
	
	UPROPERTY()
	TArray<TObjectPtr<UTexture2D>> TextureSet;
	
};
