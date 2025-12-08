#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Interface/Interactable.h"
#include "InteractionBase.generated.h"

class APlayerControllerBase;
class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
class APlayerCharacter;
class UCameraComponent;
class UBoxComponent;
enum EInteractionType : uint8;

UCLASS()
class NOBODY_API AInteractionBase : public APawn, public IInteractable
{
	GENERATED_BODY()
	
public:
	AInteractionBase();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	
public:
	virtual void Interact_Implementation() override;

protected:
	/** 시작 ActorSequence의 끝부분에 Trigger로 호출되는 이벤트입니다. */
	UFUNCTION(BlueprintCallable)
	virtual void OnStartActorSequenceEnded();

	/** 종료 ActorSequence의 끝부분에 Trigger로 호출되는 이벤트입니다. */
	UFUNCTION(BlueprintCallable)
	virtual void OnEndActorSequenceEnded();

	/** 상호작용 시작 시 ActorSequence를 재생하는 함수입니다. */
	UFUNCTION(BlueprintImplementableEvent)
	void PlayInteractionStartSequence();

	/** 상호작용 종료 시 ActorSequence를 재생하는 함수입니다. */
	UFUNCTION(BlueprintImplementableEvent)
	void PlayInteractionEndSequence();
	
	virtual void DoLook(const FInputActionValue& Value);
	virtual void DoControl(const FInputActionValue& Value);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UBoxComponent> InteractionZone;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> ControlAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	float MaxYawAngle = 45.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|수치")
	float MaxPitchAngle = 25.0f;
	
	UPROPERTY(EditAnywhere, Category = "변수|수치")
	FVector LerpLocationValue = FVector::ZeroVector;
	
	UPROPERTY()
	TObjectPtr<APlayerControllerBase> PlayerController;
	
	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player;
	
	EInteractionType InteractionType;
	
private:
	/** ActorSequence 종료 시 초기화 할 플레이어 위치 및 회전 값 */
	FVector PlayerTargetLocation = FVector::ZeroVector;
	FRotator PlayerTargetRotation = FRotator::ZeroRotator;
	
	/** Pawn 빙의 시 조정 가능한 최대 각도 */
	FRotator OriginRotation;
	float CurrentYawOffset = 0.f;
	float CurrentPitchOffset = 0.f;
	
	/** 상호작용 중인 상태를 나타내는 변수 */
	uint8 bIsInteractPossible : 1 = false;
	
public:
	FORCEINLINE EInteractionType GetInteractionType() { return this->InteractionType; }
	
};
