#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Interface/Interactable.h"
#include "InteractionBase.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
class APlayerCharacter;
class UCameraComponent;
class UBoxComponent;
enum EInteractionType : uint8;

UCLASS()
class NOBODY_API AInteractionBase : public APawn, public IInteractable
{
	GENERATED_BODY()
	
public:
	AInteractionBase();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	
public:
	virtual void Interact_Implementation() override;

protected:
	void DoLook(const FInputActionValue& Value);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UBoxComponent> InteractionZone;

	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY()
	TWeakObjectPtr<APlayerCharacter> Player;
	
	EInteractionType InteractionType;
	
public:
	FORCEINLINE EInteractionType GetInteractionType() { return this->InteractionType; }
	
};
