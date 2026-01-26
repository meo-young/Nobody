#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "EnemyBase.generated.h"

class UEventSpawnManager;
class AInteractionBase;

USTRUCT()
struct FEnemyStepInfo
{
	GENERATED_BODY()
	
	/** 재생할 애니메이션 시퀀스입니다. */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimSequence> AnimationSequence;
    
	/** 이동할 목표 Transform입니다. */
	UPROPERTY(EditAnywhere)
	FTransform TargetTransform;
};

UCLASS()
class NOBODY_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
public:
	/** 스텝 시스템을 시작하는 함수입니다. */
	void StartStepSystem();
	
	/** 스텝 시스템을 일시정지 하는 함수입니다. */
	void PauseStepSystem();
    
	/** 스텝 시스템 중단하는 함수입니다. */
	void StopStepSystem();
    
protected:
	/** 특정 스텝으로 이동시키는 함수입니다. */
	void GoToStep(int32 StepIndex);
	
	/** 다음 스텝으로 전환하는 함수입니다. */
	void MoveToNextStep();
	
	/** 스텝에 해당하는 애니메이션을 재생하는 함수입니다. */
	void PlayStepAnimation(uint8 InStepIndex);
	
	/** 스텝에 해당하는 위치로 이동시키는 함수입니다. */
	void MoveToStepTransform(uint8 InStepIndex);
	
	/** 스폰 가능한 이벤트 목록에 추가하는 함수입니다. */
	void AddToSpawnList();
	
protected:
	/** 이벤트가 발생하는 오브젝트에 대한 변수입니다. */
	UPROPERTY(EditInstanceOnly, Category = "변수")
	TObjectPtr<AInteractionBase> InteractionObject;
	
	/** 스텝 정보 배열 */
	UPROPERTY(EditAnywhere, Category = "변수")
	TArray<FEnemyStepInfo> EnemyStepInfos;
    
	/** 스텝 전환 간격 (초) */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float StepInterval = 4.0f;
	
	/** 리스폰까지 걸리는 지연 시간입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float RespawnDelay = 30.0f;
	
	/** 이벤트가 스폰될 때 재생되는 소리입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	TObjectPtr<USoundCue> EventSpawnSound;
	
	/** 이벤트가 걸렸을 때 재생되는 소리입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	TObjectPtr<USoundCue> EventPausedSound;
	
	/** 이벤트가 스폰되는 위치입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	FVector EventSpawnLocation;
	
private:
	/** 스텝 타이머입니다. */
	FTimerHandle StepTimerHandle;
	
	/** 리스폰 시간을 측정하는 타이머입니다. */
	FTimerHandle RespawnTimerHandle;
	
	/** 현재 스텝 인덱스입니다. */
	uint8 CurrentStepIndex;
	
	/** 최대 스텝 수입니다. */
	uint8 MaxStepIndex;
	
};
