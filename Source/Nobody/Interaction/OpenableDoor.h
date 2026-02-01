#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionBase.h"
#include "OpenableDoor.generated.h"

class USpotLightComponent;
class USpringArmComponent;

UCLASS()
class NOBODY_API AOpenableDoor : public AInteractionBase
{
	GENERATED_BODY()
	
public:
	AOpenableDoor();
	virtual void BeginPlay() override;
	
public:
	bool HasPlayScarySound();
	
protected:
	virtual void OnStartActorSequenceEnded() override;
	virtual void DoControl(const FInputActionValue& Value) override;	
	virtual void CheckIfEventActivated() override;
	virtual void InitEvent() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> Door;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> Doorknob;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpotLightComponent> SpotLightComponent;
	
	/** 일정 간격마다 재생할 무서운 소리 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|소리")
	TArray<TObjectPtr<USoundCue>> ScarySounds;
	
	/** 무서운 소리 재생 위치 오프셋 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	FVector ScarySoundLocation = FVector::ZeroVector;
	
};
