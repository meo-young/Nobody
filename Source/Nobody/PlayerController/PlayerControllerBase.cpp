#include "PlayerControllerBase.h"
#include "EnhancedInputSubsystems.h"
#include "Nobody.h"
#include "InputMappingContext.h"
#include "Camera/PlayerCameraManagerBase.h"
#include "UI/Crosshair/CrosshairWidget.h"
#include "UI/Dialogue/DialogueWidget.h"
#include "UI/Manual/ManualWidget.h"
#include "UI/FadeWidget.h"

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
	
	LOG(TEXT("위젯이 생성됐습니다"))
	
	if (CrosshairWidgetClass)
	{
		if (CrosshairWidgetInstance = CreateWidget<UCrosshairWidget>(this, CrosshairWidgetClass))
		{
			CrosshairWidgetInstance->AddToViewport();
		}
	}
	
	if (DialogueWidgetClass)
	{
		if (DialogueWidgetInstance = CreateWidget<UDialogueWidget>(this, DialogueWidgetClass))
		{
			DialogueWidgetInstance->AddToViewport();
		}
	}
	
	if (ManualWidgetClass)
	{
		if (ManualWidgetInstance = CreateWidget<UManualWidget>(this, ManualWidgetClass))
		{
			ManualWidgetInstance->AddToViewport();
		}
	}
	
	if (FadeWidgetClass)
	{
		if (FadeWidgetInstance = CreateWidget<UFadeWidget>(this, FadeWidgetClass))
		{
			FadeWidgetInstance->AddToViewport();
		}
	}
}
