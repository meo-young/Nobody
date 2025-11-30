#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Interface/Interactable.h"
#include "InteractionBase.generated.h"

enum EInteractionType : uint8;

UCLASS()
class NOBODY_API AInteractionBase : public ATriggerBox, public IInteractable
{
	GENERATED_BODY()
	
public:
	AInteractionBase();
	
public:
	virtual void Interact_Implementation() override;
	
protected:
	EInteractionType InteractionType;
	
public:
	FORCEINLINE EInteractionType GetInteractionType() { return this->InteractionType; }
	
};
