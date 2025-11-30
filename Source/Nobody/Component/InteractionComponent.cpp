#include "Component/InteractionComponent.h"
#include "Nobody.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Define/Define.h"
#include "Enum/EInteractType.h"
#include "Interaction/InteractionBase.h"
#include "Interface/Interactable.h"
#include "PlayerController/PlayerControllerBase.h"
#include "UI/Crosshair/CrosshairWidget.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		CameraComponent = Player->GetCameraComponent();	
		
		if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(Player->GetController()))
		{
			CrosshairWidget = PlayerController->GetCrosshairWidget();
		}
	}
	
	CollisionParams.AddIgnoredActor(GetOwner());
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!CameraComponent)
	{
		LOG(TEXT("카메라 컴포넌트가 유효하지 않습니다."));
		return;
	}
	
	// 카메라의 위치 기준으로 상호작용이 가능한 물체가 있는지 LineTrace를 수행합니다.
	const FVector StartLocation = CameraComponent->GetComponentLocation();
	const FVector EndLocation = StartLocation + CameraComponent->GetForwardVector() * InteractionDistance;
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_INTERACTION, CollisionParams);
	
	
	// 우선 Crosshair의 이미지를 "기본"으로 설정합니다. 
	CrosshairWidget->SetCrosshair(EInteractionType::None);
	
	
	// 상호작용 물체가 있는 경우 Crosshair의 이미지를 변경합니다.
	if (AActor* HitActor = HitResult.GetActor())
	{
		if (AInteractionBase* InteractionBase = Cast<AInteractionBase>(HitActor))
		{
			CrosshairWidget->SetCrosshair(InteractionBase->GetInteractionType());
		}
	}
	
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 0.5f);
}

void UInteractionComponent::ExecuteInteractIfPossible()
{
	if (AActor* HitActor = HitResult.GetActor())
	{
		if (HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			IInteractable::Execute_Interact(HitActor);
		}
	}
}

