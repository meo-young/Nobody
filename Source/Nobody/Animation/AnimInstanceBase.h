#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"

class UCharacterMovementComponent;
class ACharacterBase;

UCLASS()
class NOBODY_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	/** 변수를 초기화하고 컴포넌트를 캐싱하는 함수입니다. */
	virtual void NativeInitializeAnimation() override;
	
	/** 매 프레임 업데이트를 수행하는 함수입니다. */
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	/** 스레드 안전한 상태로 매 프레임 업데이트를 수행하는 함수입니다. */
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	/** 캐릭터의 이동 방향에 대한 변수입니다. */
	UPROPERTY(BlueprintReadOnly, Category = "변수")
	FVector2D MoveDirection;

	/** 캐릭터의 이동 속도에 대한 변수입니다. */
	UPROPERTY(BlueprintReadOnly, Category = "변수")
	float MoveSpeed;

private:
	/** 이 애니메이션 인스턴스가 소유하고 있는 캐릭터입니다. */
	UPROPERTY()
	TObjectPtr<ACharacterBase> Player = nullptr;

	/** 캐릭터의 이동 컴포넌트입니다. */
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent = nullptr;
};