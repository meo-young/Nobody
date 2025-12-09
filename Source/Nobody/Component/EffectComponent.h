#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EffectComponent.generated.h"

class UCameraComponent;
class UBlurWidget;

USTRUCT()
struct FEffectIntensity
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	float FilmGrainIntensity = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float ChromaticIntensity = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float BlurIntensity = 0.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOBODY_API UEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEffectComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void UpdateEffect();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	FEffectIntensity EffectMaxIntensity;

	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	float EffectUpInterpTime = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	float EffectDownInterpTime = 4.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	uint8 bIsEffectEnabled : 1 = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UBlurWidget> BlurWidgetClass;

	UPROPERTY()
	TObjectPtr<UBlurWidget> BlurWidgetInstance;

private:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;
	
	FTimerHandle EffectTimerHandle;

	FEffectIntensity CurrentEffectIntensity;
	FEffectIntensity EffectMinIntensity;

	float AccumulatedTime = 0.0f;

public:
	FORCEINLINE void SetEffectEnable(const bool bEnable) { bIsEffectEnabled = bEnable; }
	
};
