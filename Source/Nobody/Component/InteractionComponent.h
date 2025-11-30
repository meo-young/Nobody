#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class UCrosshairWidget;
class IInteractable;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOBODY_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	void ExecuteInteractIfPossible();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|거리")
	float InteractionDistance = 100.0f;
	
private:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY()
	TObjectPtr<UCrosshairWidget> CrosshairWidget;
	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
		
};
