#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pill.generated.h"

class UTitleWidget;

UENUM(BlueprintType)
enum class EPillType : uint8
{
	Start   UMETA(DisplayName = "Start"),
	Setting UMETA(DisplayName = "Setting"),
	Quit    UMETA(DisplayName = "Quit"),
};

UCLASS()
class NOBODY_API APill : public AActor
{
	GENERATED_BODY()

public:
	APill();
	virtual void BeginPlay() override;

public:
	/** 마우스 호버 시 호출되는 함수입니다. */
	UFUNCTION()
	void OnBeginCursorOverFunction(AActor* TouchedActor);

	/** 마우스 호버 해제 시 호출되는 함수입니다. */
	UFUNCTION()
	void OnEndCursorOverFunction(AActor* TouchedActor);

	/** 마우스 클릭 시 호출되는 함수입니다. */
	UFUNCTION()
	void OnClickedFunction(AActor* TouchedActor, FKey ButtonPressed);

private:
	/** 알약 타입에 따른 동작을 실행하는 함수입니다. */
	void ExecutePillAction();

	/** 문구 위젯을 표시하는 함수입니다. */
	void ShowLabel(bool bShow);

public:
	UPROPERTY(VisibleAnywhere, Category = "컴포넌트")
	TObjectPtr<UStaticMeshComponent> PillMesh;

	UPROPERTY(EditAnywhere, Category = "변수")
	EPillType PillType;

	UPROPERTY(EditAnywhere, Category = "변수")
	FText LabelText;

	UPROPERTY(EditAnywhere, Category = "변수")
	TSubclassOf<UTitleWidget> LabelWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UTitleWidget> LabelWidgetInstance;
};