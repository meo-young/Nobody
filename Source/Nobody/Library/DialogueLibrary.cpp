#include "Library/DialogueLibrary.h"
#include "Nobody.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/DialogueSubsystem.h"

void UDialogueLibrary::ShowDialogue(const UObject* WorldContextObject, EVoice InVoice)
{
	if (UDialogueSubsystem* DialogueSubsystem = GetDialogueSubsystem(WorldContextObject))
	{
		DialogueSubsystem->ShowDialogue(InVoice);
	}
}

void UDialogueLibrary::ShowDialogue(const UObject* WorldContextObject, EVoice InVoice, const FVector& InLocation)
{
	if (UDialogueSubsystem* DialogueSubsystem = GetDialogueSubsystem(WorldContextObject))
	{
		DialogueSubsystem->ShowDialogue(InVoice, InLocation);
	}
}

UDialogueSubsystem* UDialogueLibrary::GetDialogueSubsystem(const UObject* WorldContextObject)
{
	if (const UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		if (UDialogueSubsystem* DialogueSubsystem = GameInstance->GetSubsystem<UDialogueSubsystem>())
		{
			return DialogueSubsystem;
		}
		else
		{
			LOG(TEXT("SoundSubsystem이 유효하지 않습니다"));
			return nullptr;
		}
	}
	else
	{
		LOG(TEXT("GameInstance가 유효하지 않습니다"));
		return nullptr;
	}
}
