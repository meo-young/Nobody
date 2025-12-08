#include "UI/Blur/BlurWidget.h"
#include "Components/BackgroundBlur.h"

void UBlurWidget::SetBlurIntensity(float Intensity)
{
	Blur->SetBlurStrength(Intensity);
}
