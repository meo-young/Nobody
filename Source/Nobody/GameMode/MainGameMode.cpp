#include "MainGameMode.h"
#include "Character/Player/PlayerCharacter.h"
#include "PlayerController/PlayerControllerBase.h"

AMainGameMode::AMainGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerCharacter> BP_Player(TEXT("/Game/_Nobody/Blueprint/Character/Player/BP_Player"));
	if (BP_Player.Succeeded())
	{
		DefaultPawnClass = BP_Player.Class;
	}
	
	static ConstructorHelpers::FClassFinder<APlayerControllerBase> BP_PlayerController(TEXT("/Game/_Nobody/Blueprint/PlayerController/BP_MainPlayerController"));
	if (BP_PlayerController.Succeeded())
	{
		PlayerControllerClass = BP_PlayerController.Class;
	}
}
