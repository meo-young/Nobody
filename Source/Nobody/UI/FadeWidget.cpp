#include "UI/FadeWidget.h"

void UFadeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetRenderOpacity(0.0f);
}
