#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "CrosshairWidget.generated.h"

class UImage;
enum EInteractionType : uint8;

UCLASS()
class NOBODY_API UCrosshairWidget : public UWidgetBase
{
	GENERATED_BODY()
    
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
    
public:
	/** 상호작용 타입에 따라 크로스헤어 이미지를 변경하는 함수입니다. */
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
    
	UPROPERTY(EditDefaultsOnly, Category = "변수|텍스처")
	TObjectPtr<UTexture2D> SoundTexture;
    
private:
	/** 상호작용 타입에 대응하는 텍스처 배열 변수입니다. */
	UPROPERTY()
	TArray<TObjectPtr<UTexture2D>> TextureSet;
    
};