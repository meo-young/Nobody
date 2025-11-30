#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionBase.h"
#include "OpenableDoor.generated.h"

UCLASS()
class NOBODY_API AOpenableDoor : public AInteractionBase
{
	GENERATED_BODY()
	
public:
	AOpenableDoor();
	
public:
	virtual void Interact_Implementation() override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayActorSequence();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> Door;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> Doorknob;
	
};
