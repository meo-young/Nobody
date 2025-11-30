#include "PlayerControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "Nobody.h"
#include "InputMappingContext.h"

APlayerControllerBase::APlayerControllerBase()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Player(TEXT("/Game/_Nobody/Input/IMC_Player"));
	if (IMC_Player.Succeeded())
	{
		MappingContext = IMC_Player.Object;
	}
	
	static ConstructorHelpers::FClassFinder<UInputMappingContext> BP_PlayerCameraManager(TEXT("/Game/_Nobody/Blueprint/Camera/BP_PlayerCameraManager"));
	if (BP_PlayerCameraManager.Succeeded())
	{
		PlayerCameraManagerClass = BP_PlayerCameraManager.Class;
	}
}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}
	else
	{
		LOG(TEXT("MappingContext 추가 실패했습니다."));
	}
}
