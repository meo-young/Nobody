#include "Component/InteractionComponent.h"
#include "Nobody.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Define/Define.h"

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
	
	const FVector StartLocation = CameraComponent->GetComponentLocation();
	const FVector EndLocation = StartLocation + CameraComponent->GetForwardVector() * InteractionDistance;
	
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_INTERACTION, CollisionParams);
	
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 0.5f);
}

