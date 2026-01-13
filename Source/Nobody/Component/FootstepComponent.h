#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootstepComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOBODY_API UFootstepComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFootstepComponent();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndReason) override;
	
public:
	/** 발걸음 소리 재생을 시작하는 함수입니다. */
	void StartFootstep();

	/** 발걸음 소리 재생을 중지하는 함수입니다. */
	void StopFootstep();

protected:
	/** 발걸음 소리를 재생하는 함수입니다. */
	void PlayFootstepSound();

private:
	/** 발걸음 타이머 핸들입니다. */
	FTimerHandle FootstepTimerHandle;

	/** LineTrace용 변수입니다. */
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	
	/** 발걸음 재생 간격을 나타내는 변수입니다. (기본값 0.5초) */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float FootstepInterval = 0.5f;

	/** LineTrace 거리를 나타내는 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float TraceDistance = 200.0f;
	
};
