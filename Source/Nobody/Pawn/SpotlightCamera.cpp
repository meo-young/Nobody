#include "SpotlightCamera.h"

#include "EnhancedInputComponent.h"
#include "Nobody.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"

USpotlightCameraComponent::USpotlightCameraComponent()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	//CameraComponent->SetupAttachment(GetRootComponent());

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(CameraComponent);
	
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(SpringArmComponent);
	SpotLight->SetLightingChannels(false, true, false);
	SpotLight->SetVisibility(false);
}

/*
void ASpotlightCamera::SetupPlayerInputComponent(class UInputComponent* IC)
{
	Super::SetupPlayerInputComponent(IC);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(IC))
	{
		if (LookAction)
		{
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::DoLook);
		}
	}
}

void ASpotlightCamera::DoLook(const FInputActionValue& Value)
{
	LOG(TEXT("호출"));
}
*/


