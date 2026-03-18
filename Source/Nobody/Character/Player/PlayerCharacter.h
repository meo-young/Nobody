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

UENUM(BlueprintType)
enum class EJumpScareType : uint8
{
	Doll = 0,
	Mannequin = 1
};

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
	void ExecuteDeathSequence(EJumpScareType JumpScareType);
	void SetInputEnable(const bool bEnable);

	/** 배터리를 1칸 소모합니다. 배터리가 남아있으면 true를 반환합니다. */
	bool UseBattery();

	/** 배터리를 최대치로 충전합니다. */
	void RechargeBattery();
	
protected:
	/** 이동에 대한 입력을 처리하는 함수입니다. */
	void DoMove(const FInputActionValue& InputActionValue);
	
	/** 시점 회전에 대한 입력을 처리하는 함수입니다. */
	void DoLook(const FInputActionValue& InputActionValue);
	
	/** 상호작용에 대한 입력을 처리하는 함수입니다. */
	void DoInteract(const FInputActionValue& InputActionValue);
	
	UFUNCTION(BlueprintNativeEvent)
	void ShowDeadJumpScare(EJumpScareType JumpScareType);
	
	void ShowDeadJumpScare_Implementation(EJumpScareType JumpScareType);
	
private:
	/** 캐릭터의 초기 설정을 담당하는 함수입니다. */
	void InitCharacterSetting();
	
	void MergeAndApplyMeshes();

protected:
#pragma region Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> DollJumpScareMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> MannequinJumpScareMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USpotLightComponent> JumpScareLight;
	
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
#pragma endregion
	
	
#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputAction> InteractAction;
#pragma endregion
	
	// CameraBoom의 CameraRotationLagSpeed와 동일한 값으로 설정
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float RotationLagSpeed = 17.0f;
	
	/** 손전등 최대 배터리 칸수를 나타내는 변수입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "변수")
	int32 MaxBatteryCount = 3;

	uint8 bIsHiding : 1 = false;

	UPROPERTY()
	TObjectPtr<APlayerControllerBase> PlayerController;
	
private:
	uint8 bIsInputEnabled : 1 = false;

	/** 현재 남은 배터리 칸수를 나타내는 변수입니다. */
	int32 CurrentBatteryCount = 3;

public:
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE UVoiceComponent* GetVoiceComponent() const { return VoiceComponent; }
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArmComponent; }
	FORCEINLINE void SetIsHiding(const bool bHiding) { bIsHiding = bHiding; }
	FORCEINLINE bool GetIsHiding() const { return bIsHiding; }
	FORCEINLINE int32 GetBatteryCount() const { return CurrentBatteryCount; }
	FORCEINLINE int32 GetMaxBatteryCount() const { return MaxBatteryCount; }
	
};
