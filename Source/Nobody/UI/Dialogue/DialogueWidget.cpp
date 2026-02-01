#include "UI/Dialogue/DialogueWidget.h"
#include "Components/TextBlock.h"

void UDialogueWidget::SetText(const FString& Text)
{
	DialogueText->SetText(FText::FromString(Text));
}
