#include "UI/WidgetBase.h"

void UWidgetBase::ShowWidget()
{
	if (!IsInViewport())
	{
		AddToViewport();
	}
}

void UWidgetBase::HideWidget()
{
	if (!IsInViewport())
	{
		RemoveFromParent();
	}
}
