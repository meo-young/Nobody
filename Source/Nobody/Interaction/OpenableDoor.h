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
	
protected:
	virtual void OnStartActorSequenceEnded() override;
	virtual void DoControl(const FInputActionValue& Value) override;	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> Door;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> Doorknob;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpotLightComponent> SpotLightComponent;
	
};
