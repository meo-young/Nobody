#include "UI/WidgetBase.h"
#include "Nobody.h"

void UWidgetBase::ShowWidget()
{
	if (!IsInViewport())
	{
		LOG(TEXT("위젯을 표시합니다"))
		AddToViewport();
	}
}

void UWidgetBase::HideWidget()
{
	if (IsInViewport())
	{
		LOG(TEXT("위젯을 숨깁니다"))
		RemoveFromParent();
	}
}
