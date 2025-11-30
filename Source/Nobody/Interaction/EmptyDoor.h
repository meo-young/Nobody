#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionBase.h"
#include "EmptyDoor.generated.h"

UCLASS()
class NOBODY_API AEmptyDoor : public AInteractionBase
{
	GENERATED_BODY()
	
public:
	AEmptyDoor();
	
public:
	virtual void Interact_Implementation() override;
	
};
