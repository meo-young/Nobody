#include "UI/TitleWidget.h"
#include "Components/TextBlock.h"

void UTitleWidget::SetText(const FString& NewTitle)
{ 
	if (TitleText)
	{
		TitleText->SetText(FText::FromString(NewTitle));
		ShowWidget();
	}
}
