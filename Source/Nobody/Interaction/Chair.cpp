#include "Interaction/Chair.h"
#include "EnhancedInputComponent.h"
#include "Enum/EInteractType.h"
#include "PlayerController/PlayerControllerBase.h"
#include "UI/Manual/ManualWidget.h"
#include "Camera/CameraComponent.h"
#include "Component/InteractionComponent.h"
#include "Define/Define.h"

AChair::AChair()
{
	InteractionType = EInteractionType::Hide;
	
	ChairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMesh"));
	ChairMesh->SetupAttachment(Root);
	
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void AChair::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionComponent->SetTickEnabled(false);
}


void AChair::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InteractAction)
		{
			EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::DoInteract);
		}
	}
}

void AChair::InitEvent()
{
	Super::InitEvent();
	
	PlayerController->GetManualWidget()->SetLeftClickAction(TEXT("재생하기"));
	PlayerController->GetManualWidget()->SetRightClickAction(TEXT("물러서기"));
	PlayerController->GetManualWidget()->ShowWidget();
}

void AChair::OnStartActorSequenceEnded()
{
	Super::OnStartActorSequenceEnded();
	
	InteractionComponent->SetTickEnabled(true);
}

void AChair::OnEndActorSequenceEnded()
{
	Super::OnEndActorSequenceEnded();
	
	InteractionComponent->SetTickEnabled(false);
}

void AChair::DoInteract(const FInputActionValue& InputActionValue)
{
	InteractionComponent->ExecuteInteractIfPossible();
}
