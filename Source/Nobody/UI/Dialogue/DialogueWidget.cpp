#include "UI/Dialogue/DialogueWidget.h"
#include "Components/TextBlock.h"

void UDialogueWidget::SetText(const FString& Text, const float DisplayDuration)
{
	// 텍스트를 설정합니다.
	DialogueText->SetText(FText::FromString(Text));
    
	// 위젯을 표시합니다.
	ShowWidget();
    
	// 기존 타이머가 활성화되어 있다면 초기화합니다.
	if (GetWorld()->GetTimerManager().IsTimerActive(HideTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
	}
    
	// DisplayDuration 후에 위젯을 숨기도록 타이머를 설정합니다.
	GetWorld()->GetTimerManager().SetTimer(
		HideTimerHandle,
		this,
		&UWidgetBase::HideWidget,
		DisplayDuration,
		false
	);
}
