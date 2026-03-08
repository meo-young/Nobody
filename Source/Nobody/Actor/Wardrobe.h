#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character/Player/PlayerCharacter.h"
#include "Wardrobe.generated.h"

class ABed;

UCLASS()
class NOBODY_API AWardrobe : public AActor
{
	GENERATED_BODY()

public:
	AWardrobe();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/** 이벤트를 시작합니다. LevelSequence를 재생하고 Bed 은신 감시를 시작합니다. */
	UFUNCTION(BlueprintCallable)
	void StartEvent();

	/** LevelSequence 끝부분에서 Blueprint Trigger로 호출되는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	void OnEventSequenceEnded();

protected:
	/** 이벤트 LevelSequence를 재생합니다. Blueprint에서 구현합니다. */
	UFUNCTION(BlueprintImplementableEvent)
	void PlayEventSequence();
	
	/** 이벤트 LevelSequence를 중단합니다. Blueprint에서 구현합니다. */
	UFUNCTION(BlueprintImplementableEvent)
	void StopEventSequence();

protected:
#pragma region Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UStaticMeshComponent> WardrobeDoorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UStaticMeshComponent> WardrobeDrawerMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UStaticMeshComponent> WardrobeBodyMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "컴포넌트")
	TObjectPtr<UStaticMeshComponent> WardrobeDoorHandleMesh;
#pragma endregion

	/** 이벤트 클리어 조건이 되는 Bed 액터입니다. */
	UPROPERTY(EditAnywhere, Category = "변수|이벤트")
	TObjectPtr<ABed> BedActor;

	/** 이벤트 클리어에 필요한 Bed 연속 은신 시간 (초)입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|이벤트")
	float RequiredHidingDuration = 4.0f;

	/** 이벤트 실패 시 재생할 JumpScare 타입입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수|이벤트")
	EJumpScareType JumpScareType = EJumpScareType::Doll;

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player;

	/** 이전 프레임의 숨기 상태 (변화 감지용) */
	bool bWasHiding = false;
	
	/** Bed 연속 은신 누적 시간입니다. */
	float HidingAccumulatedTime = 0.0f;

	/** 이벤트가 진행 중인지를 나타내는 변수입니다. */
	uint8 bEventActive : 1 = false;

	/** 이벤트 클리어 조건이 충족됐는지를 나타내는 변수입니다. */
	uint8 bEventCleared : 1 = false;
};