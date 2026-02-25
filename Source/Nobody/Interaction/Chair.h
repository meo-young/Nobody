#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionBase.h"
#include "Chair.generated.h"

class UInteractionComponent;

UCLASS()
class NOBODY_API AChair : public AInteractionBase
{
	GENERATED_BODY()
	
public:
	AChair();
	virtual	void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
protected:
	virtual void InitEvent() override;
	virtual void OnStartActorSequenceEnded() override;
	virtual void OnEndActorSequenceEnded() override;
	
	/** 상호작용에 대한 입력을 처리하는 함수입니다. */
	void DoInteract(const FInputActionValue& InputActionValue);	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> ChairMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UInteractionComponent> InteractionComponent;
	
};
