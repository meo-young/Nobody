#include "BaseGameMode.h"

#include "Subsystem/DialogueSubsystem.h"
#include "Subsystem/SoundSubsystem.h"

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (USoundSubsystem* SoundSubsystem = GetGameInstance()->GetSubsystem<USoundSubsystem>())
	{
		SoundSubsystem->Init();
	}
	
	if (UDialogueSubsystem* DialogueSubsystem = GetGameInstance()->GetSubsystem<UDialogueSubsystem>())
	{
		DialogueSubsystem->Init();
	}
}
