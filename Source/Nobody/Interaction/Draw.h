#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionBase.h"
#include "Draw.generated.h"

class UChildActorComponent;
class UInteractionComponent;

UCLASS()
class NOBODY_API ADraw : public AInteractionBase
{
	GENERATED_BODY()
	
public:
	ADraw();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
public:
	virtual void Interact_Implementation() override;
	void InitCameraRotation();
	
protected:
	virtual void OnStartActorSequenceEnded() override;
	virtual void OnEndActorSequenceEnded() override;
	void DoInteract(const FInputActionValue& InputActionValue);
	
protected:
	virtual void DoControl(const FInputActionValue& Value) override;
	virtual void InitEvent() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UStaticMeshComponent> DrawBodyMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UChildActorComponent> DrawHandleComponent1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UChildActorComponent> DrawHandleComponent2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UChildActorComponent> DrawHandleComponent3;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UInteractionComponent> InteractionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> InteractAction;
	
};
