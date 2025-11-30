#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Interface/Interactable.h"
#include "InteractionBase.generated.h"

class APlayerCharacter;
class UCameraComponent;
class UBoxComponent;
enum EInteractionType : uint8;

UCLASS()
class NOBODY_API AInteractionBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	AInteractionBase();
	
public:
	virtual void Interact_Implementation() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UBoxComponent> InteractionZone;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY()
	TWeakObjectPtr<APlayerCharacter> Player;
	
	EInteractionType InteractionType;
	
public:
	FORCEINLINE EInteractionType GetInteractionType() { return this->InteractionType; }
	
};
