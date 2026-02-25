#include "CrosshairWidget.h"
#include "Nobody.h"
#include "Components/Image.h"

void UCrosshairWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	// 에디터 프리뷰용으로 TextureSet을 초기화합니다.
	TextureSet.Reset();
	TextureSet.Emplace(NoneTexture);
	TextureSet.Emplace(InspectTexture);
	TextureSet.Emplace(HideTexture);
	TextureSet.Emplace(SoundTexture);
}

void UCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 런타임에 TextureSet을 초기화합니다.
	TextureSet.Reset();
	TextureSet.Emplace(NoneTexture);
	TextureSet.Emplace(InspectTexture);
	TextureSet.Emplace(HideTexture);
	TextureSet.Emplace(SoundTexture);
}

void UCrosshairWidget::SetCrosshair(EInteractionType InteractionType)
{
	if (UTexture2D* TargetTexture = TextureSet[static_cast<uint8>(InteractionType)])
	{
		Crosshair_Image->SetBrushFromTexture(TargetTexture);
	}
}