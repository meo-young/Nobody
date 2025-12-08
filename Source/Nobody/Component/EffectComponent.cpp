#include "Component/EffectComponent.h"
#include "Nobody.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "UI/Blur/BlurWidget.h"

UEffectComponent::UEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	static ConstructorHelpers::FClassFinder<UBlurWidget> WBP_Blur(TEXT("/Game/_Nobody/UI/WBP_Blur"));
	if (WBP_Blur.Succeeded())
	{
		BlurWidgetClass = WBP_Blur.Class;
	}
}

void UEffectComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (BlurWidgetClass)
	{
		BlurWidgetInstance = CreateWidget<UBlurWidget>(GetWorld(), BlurWidgetClass);
		if (BlurWidgetInstance)
		{
			BlurWidgetInstance->ShowWidget();
		}
	}

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		CameraComponent = Player->GetCameraComponent();
	}
	
	bIsEffectEnabled = true;
}

void UEffectComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateEffect();
}

void UEffectComponent::UpdateEffect()
{
	if (bIsEffectEnabled)
	{
		AccumulatedTime += GetWorld()->GetDeltaSeconds() / EffectUpInterpTime;
	}
	else
	{
		AccumulatedTime -= GetWorld()->GetDeltaSeconds() / EffectDownInterpTime;
	}

	const float Alpha = FMath::Clamp(AccumulatedTime, 0.0f, 1.0f);
	
	CurrentEffectIntensity.FilmGrainIntensity =
		FMath::Lerp(EffectMinIntensity.FilmGrainIntensity, EffectMaxIntensity.FilmGrainIntensity, Alpha);

	CurrentEffectIntensity.ChromaticIntensity =
		FMath::Lerp(EffectMinIntensity.ChromaticIntensity, EffectMaxIntensity.ChromaticIntensity, Alpha);

	CurrentEffectIntensity.BlurIntensity =
		FMath::Lerp(EffectMinIntensity.BlurIntensity, EffectMaxIntensity.BlurIntensity, Alpha);


	CameraComponent->PostProcessSettings.FilmGrainIntensity = CurrentEffectIntensity.FilmGrainIntensity;
	CameraComponent->PostProcessSettings.SceneFringeIntensity = CurrentEffectIntensity.ChromaticIntensity;
	BlurWidgetInstance->SetBlurIntensity(CurrentEffectIntensity.BlurIntensity);
}

