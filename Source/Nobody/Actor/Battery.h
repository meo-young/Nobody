#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Battery.generated.h"

class UBoxComponent;

UCLASS()
class NOBODY_API ABattery : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ABattery();

public:
	virtual void Interact_Implementation() override;
	virtual EInteractionType GetInteractionType() override;

	void EnableInteraction();
	void DisableInteraction();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UStaticMeshComponent> BatteryMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UBoxComponent> InteractionZone;
	
private:
	EInteractionType InteractionType;

};
