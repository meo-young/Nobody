#include "Interaction/Draw.h"

#include "Actor/DrawHandle.h"
#include "EnhancedInputComponent.h"
#include "Nobody.h"
#include "Camera/CameraComponent.h"
#include "Component/InteractionComponent.h"
#include "Components/BoxComponent.h"
#include "Define/Define.h"
#include "Enum/EInteractType.h"
#include "PlayerController/PlayerControllerBase.h"
#include "UI/Manual/ManualWidget.h"

ADraw::ADraw()
{
	InteractionType = EInteractionType::Inspect;

	DrawBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Draw Body Mesh"));
	DrawBodyMesh->SetupAttachment(Root);

	DrawHandleComponent1 = CreateDefaultSubobject<UChildActorComponent>(TEXT("DrawHandle 1"));
	DrawHandleComponent1->SetChildActorClass(ADrawHandle::StaticClass());
	DrawHandleComponent1->SetupAttachment(DrawBodyMesh);

	DrawHandleComponent2 = CreateDefaultSubobject<UChildActorComponent>(TEXT("DrawHandle 2"));
	DrawHandleComponent2->SetChildActorClass(ADrawHandle::StaticClass());
	DrawHandleComponent2->SetupAttachment(DrawBodyMesh);

	DrawHandleComponent3 = CreateDefaultSubobject<UChildActorComponent>(TEXT("DrawHandle 3"));
	DrawHandleComponent3->SetChildActorClass(ADrawHandle::StaticClass());
	DrawHandleComponent3->SetupAttachment(DrawBodyMesh);
	
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

void ADraw::BeginPlay()
{
	Super::BeginPlay();

	InteractionComponent->SetTickEnabled(false);
}

void ADraw::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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

void ADraw::Interact_Implementation()
{
	Super::Interact_Implementation();
	
	bIsInteractPossible = false;
	
	LOG(TEXT("콜리전 활성화"))

	// 빙의 시 각 서랍 핸들의 상호작용 존을 활성화합니다.
	auto EnableHandle = [](UChildActorComponent* Comp)
	{
		if (Comp)
		{
			if (ADrawHandle* Handle = Cast<ADrawHandle>(Comp->GetChildActor()))
			{
				Handle->EnableInteraction();
			}
		}
	};

	EnableHandle(DrawHandleComponent1);
	EnableHandle(DrawHandleComponent2);
	EnableHandle(DrawHandleComponent3);
}

void ADraw::InitCameraRotation()
{
	CameraComponent->SetRelativeRotation(OriginRotation);
	CurrentYawOffset = 0.f;
	CurrentPitchOffset = 0.f;
}

void ADraw::OnStartActorSequenceEnded()
{
	Super::OnStartActorSequenceEnded();
	
	InteractionComponent->SetTickEnabled(true);
}

void ADraw::OnEndActorSequenceEnded()
{
	InteractionComponent->SetTickEnabled(false);
	
	Super::OnEndActorSequenceEnded();
}

void ADraw::DoInteract(const FInputActionValue& InputActionValue)
{
	bIsInteractPossible = false;
	
	InteractionComponent->ExecuteInteractIfPossible();
	
	LOG(TEXT("마우스 좌클릭"))
	PlayerController->GetManualWidget()->HideWidget();
}

void ADraw::DoControl(const FInputActionValue& Value)
{
	Super::DoControl(Value);
	
	LOG(TEXT("마우스 우클릭"))
	
	// 탈출 시 각 서랍 핸들의 상호작용 존을 비활성화합니다.
	auto DisableHandle = [](UChildActorComponent* Comp)
	{
		if (Comp)
		{
			if (ADrawHandle* Handle = Cast<ADrawHandle>(Comp->GetChildActor()))
			{
				Handle->DisableInteraction();
			}
		}
	};

	DisableHandle(DrawHandleComponent1);
	DisableHandle(DrawHandleComponent2);
	DisableHandle(DrawHandleComponent3);
}

void ADraw::InitEvent()
{
	Super::InitEvent();
	
	PlayerController->GetManualWidget()->SetLeftClickAction(TEXT("열어보기"));
	PlayerController->GetManualWidget()->SetRightClickAction(TEXT("물러서기"));
	PlayerController->GetManualWidget()->ShowWidget();
}
