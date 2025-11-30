#include "CrosshairWidget.h"
#include "Nobody.h"
#include "Components/Image.h"

void UCrosshairWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	LOG(TEXT("호출"));
	
	TextureSet.Emplace(NoneTexture);
	TextureSet.Emplace(InspectTexture);
	TextureSet.Emplace(HideTexture);
}

void UCrosshairWidget::SetCrosshair(EInteractionType InteractionType)
{
	if (UTexture2D* TargetTexture = TextureSet[static_cast<uint8>(InteractionType)])
	{
		Crosshair_Image->SetBrushFromTexture(TargetTexture);
	}
}
