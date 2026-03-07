#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wardrobe.generated.h"

class APlayerCharacter;

UCLASS()
class NOBODY_API AWardrobe : public AActor
{
	GENERATED_BODY()

public:
	AWardrobe();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

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

	/**
	 * 각 단계 진입 전 대기 시간 (초) 배열입니다.
	 * 인덱스 0 = 1단계 열리기까지 대기, 인덱스 1 = 2단계까지 대기, ...
	 * 기본값: {5, 3, 4, 3, 2, 2, 1}
	 */
	UPROPERTY(EditDefaultsOnly, Category = "변수|단계")
	TArray<float> OpeningDelays = {5.f, 3.f, 4.f, 3.f, 2.f, 2.f, 1.f};

	/**
	 * 각 단계별 WardrobeDoorHandleMesh의 Y축 이동 거리 (7개)
	 * 각 값은 해당 단계에서 이전 단계로부터 이동할 거리입니다.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "변수|단계")
	TArray<float> StageOffsets = {10.f, 15.f, 7.f, 5.f, 15.f, 7.f, 8.f};

	/** 숨은 상태에서 역단계로 진행할 때 각 단계 사이 대기 시간 (초) */
	UPROPERTY(EditDefaultsOnly, Category = "변수|단계")
	float ClosingStageDelay = 1.5f;

	/** 문이 이동하는 속도 */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float MoveSpeed = 50.0f;

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player;

	/** 문이 닫혔을 때의 원점 위치 */
	FVector OriginalLocation;

	/** 현재 열린 단계 (0 = 닫힘, MaxStage = 완전히 열림) */
	int32 CurrentStage = 0;

	static constexpr int32 MaxStage = 7;

	/** 이전 프레임의 숨기 상태 (변화 감지용) */
	bool bWasHiding = false;

	/** 현재 문이 목표 위치로 이동 중인지 여부 */
	bool bIsMoving = false;

	/** 단계 진행 타이머 핸들 */
	FTimerHandle StageTimerHandle;

	/** Stage에 해당하는 월드 위치를 반환합니다. */
	FVector GetStageLocation(int32 Stage) const;

	/** 현재 상태(숨김/비숨김, 현재 단계)에 따라 다음 단계 타이머를 스케줄합니다. */
	void ScheduleNextStep();

	/** 다음 열기 단계로 진행합니다. */
	void AdvanceStage();

	/** 이전 닫기 단계로 되돌아갑니다. */
	void RetreatStage();
};