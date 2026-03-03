#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class USpotLightComponent;
class UFootstepComponent;
class APlayerControllerBase;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class UInteractionComponent;
class UEffectComponent;
class UVoiceComponent;

UCLASS()
class NOBODY_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void SetEffectEnable(const bool bEnable);
	void ExecuteDeathSequence();
	
protected:
	/** 이동에 대한 입력을 처리하는 함수입니다. */
	void DoMove(const FInputActionValue& InputActionValue);
	
	/** 시점 회전에 대한 입력을 처리하는 함수입니다. */
	void DoLook(const FInputActionValue& InputActionValue);
	
	/** 상호작용에 대한 입력을 처리하는 함수입니다. */
	void DoInteract(const FInputActionValue& InputActionValue);
	
	UFUNCTION(BlueprintNativeEvent)
	void ShowDeadJumpScare();
	
	void ShowDeadJumpScare_Implementation();
	
private:
	/** 캐릭터의 초기 설정을 담당하는 함수입니다. */
	void InitCharacterSetting();
	
	void MergeAndApplyMeshes();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> DollJumpScareMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USpotLightComponent> JumpScareLight;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UInteractionComponent> InteractionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UFootstepComponent> FootstepComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UVoiceComponent> VoiceComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UEffectComponent> EffectComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "변수|컴포넌트")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "변수|컴포넌트")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	// SKM_Dress, SKM_Boots 등 Body를 제외한 나머지 메시 할당
	UPROPERTY(EditDefaultsOnly, Category = "변수|메시")
	TArray<TObjectPtr<USkeletalMesh>> DollMeshesToMerge;

	// 병합된 의상 메시를 담는 컴포넌트 (DollJumpScareMesh를 Leader로 따라감)
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<USkeletalMeshComponent> DollClothMesh;

	UPROPERTY()
	TObjectPtr<APlayerControllerBase> PlayerController;
	
public:
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE UVoiceComponent* GetVoiceComponent() const { return VoiceComponent; }
	
};
