#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class APlayerControllerBase;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class UInteractionComponent;

UCLASS()
class NOBODY_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void ExecuteDeathSequence();
	
protected:
	void DoMove(const FInputActionValue& InputActionValue);
	void DoLook(const FInputActionValue& InputActionValue);
	void DoInteract(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UInteractionComponent> InteractionComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<APlayerControllerBase> PlayerController;
	
public:
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	
};
