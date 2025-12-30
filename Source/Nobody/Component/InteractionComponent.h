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
	/** 상호작용이 가능한 상태일 때 상호작용을 수행하는 함수입니다. */
	void ExecuteInteractIfPossible();
	
protected:
	/** 상호작용이 가능한 최대 거리 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|거리")
	float InteractionDistance = 100.0f;
	
private:
	/** 컴포넌트 오너의 카메라 컴포넌트를 캐싱한 변수입니다. */
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;
	
	/** 상호작용이 가능할 때 변경하기 위한 Crosshair UI 변수입니다. */
	UPROPERTY()
	TObjectPtr<UCrosshairWidget> CrosshairWidget;
	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
		
};
