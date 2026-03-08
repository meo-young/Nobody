#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "DrawHandle.generated.h"

class UChildActorComponent;
class UInteractionComponent;
class ADraw;
class ABattery;
class APlayerCharacter;
class APlayerControllerBase;
struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class UBoxComponent;

UCLASS()
class NOBODY_API ADrawHandle : public APawn, public IInteractable
{
	GENERATED_BODY()
	
public:
	ADrawHandle();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
public:
	virtual void Interact_Implementation() override;
	virtual EInteractionType GetInteractionType() override;

	/** Draw에 빙의 시 InteractionZone을 활성화합니다. */
	void EnableInteraction();

	/** Draw에서 탈출 시 InteractionZone을 비활성화합니다. */
	void DisableInteraction();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayInteractionStartSequence();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayInteractionEndSequence();
	
	/** 시작 ActorSequence의 끝부분에 Trigger로 호출되는 이벤트입니다. */
	UFUNCTION(BlueprintCallable)
	void OnStartActorSequenceEnded();
	
	/** 서랍 닫힘 시퀀스가 끝난 뒤 Blueprint Trigger로 호출됩니다. Draw로 복귀합니다. */
	UFUNCTION(BlueprintCallable)
	void OnEndActorSequenceEnded();

	void DoInteract(const FInputActionValue& InputActionValue);
	void DoLook(const FInputActionValue& InputActionValue);
	void DoBack(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UStaticMeshComponent> HandleMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UInteractionComponent> InteractionComponent;

	/** 서랍 내부에 스폰될 Battery의 ChildActorComponent입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UChildActorComponent> BatteryComponent;

	/** 0~1 사이 값. 이 확률로 서랍에 Battery가 스폰됩니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	float BatterySpawnChance = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UBoxComponent> InteractionZone;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "변수")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "변수")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "변수")
	TObjectPtr<UInputAction> BackAction;
	
	/** Yaw에 대해 회전할 수 있는 최대 각도 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	float MaxYawAngle = 45.0f;
	
	/** Pitch에 대해 회전할 수 있는 최대 각도 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	float MaxPitchAngle = 25.0f;
	
	EInteractionType InteractionType;
	
private:
	UPROPERTY()
	TObjectPtr<APlayerControllerBase> PlayerController;
	
	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player;
	
	UPROPERTY()
	TObjectPtr<ADraw> OwnerDraw;
	
	/** Pawn 빙의 시 조정 가능한 최대 각도 */
	FRotator OriginRotation;
	float CurrentYawOffset = 0.f;
	float CurrentPitchOffset = 0.f;
	
	uint8 bIsInteractPossible : 1 = false;

};
