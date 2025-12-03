#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionBase.h"
#include "OpenableDoor.generated.h"


class USpotLightComponent;
class USpringArmComponent;
class ASpotlightCamera;

UCLASS()
class NOBODY_API AOpenableDoor : public AInteractionBase
{
	GENERATED_BODY()
	
public:
	AOpenableDoor();
	virtual void BeginPlay() override;
	
public:
	virtual void Interact_Implementation() override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayActorSequence();

	UFUNCTION(BlueprintCallable)
	void PossessSpotlightCamera();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> Door;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> Doorknob;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpotLightComponent> SpotLightComponent;

private:
	
	
};
