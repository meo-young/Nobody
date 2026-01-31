#include "PlayerControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "Nobody.h"
#include "InputMappingContext.h"
#include "Camera/PlayerCameraManagerBase.h"
#include "UI/Crosshair/CrosshairWidget.h"

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

void APlayerControllerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (bActivated)
	{
		return;
	}
	
	CreateWidgetInstance();
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerControllerBase::SetInputEnable(const bool InEnable)
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InEnable)
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
			else
			{
				Subsystem->RemoveMappingContext(MappingContext);
			}
		}
	}
}

void APlayerControllerBase::CreateWidgetInstance()
{
	bActivated = true;
	
	if (CrosshairWidgetClass)
	{
		if (CrosshairWidgetInstance = CreateWidget<UCrosshairWidget>(this, CrosshairWidgetClass))
		{
			CrosshairWidgetInstance->AddToViewport();
		}
	}
}
