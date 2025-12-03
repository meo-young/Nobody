#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpotlightCamera.generated.h"

struct FInputActionValue;
class UInputAction;
class USpotLightComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class NOBODY_API USpotlightCameraComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	USpotlightCameraComponent();
	//virtual void SetupPlayerInputComponent(class UInputComponent* IC) override;

protected:
	//void DoLook(const FInputActionValue& Value);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USpotLightComponent> SpotLight;

	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> LookAction;

public:
	FORCEINLINE USpotLightComponent* GetSpotLightComponent() { return SpotLight; }

};
