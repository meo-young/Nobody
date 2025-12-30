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
	/** 이펙트 강도를 업데이트 하는 함수입니다. */
	void UpdateEffect();

protected:
	/** 최대로 조정할 수 있는 이펙트 강도에 대한 수치를 나타내는 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	FEffectIntensity EffectMaxIntensity;

	/** 이펙트 출력 효과가 보간되는 데에 소요되는 시간을 나타내는 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	float EffectUpInterpTime = 30.0f;

	/** 이펙트 감소 효과가 보간되는 데에 소요되는 시간을 나타내는 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	float EffectDownInterpTime = 4.0f;
	
	/** 이펙트 출력이 가능한 상태인지를 나타내는 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	uint8 bIsEffectEnabled : 1 = false;
	
	/** Blur 효과를 출력할 위젯 클래스를 나타내는 변수입니다. */
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
