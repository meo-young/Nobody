#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetBase.generated.h"

UCLASS()
class NOBODY_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 생성자입니다. */
	UWidgetBase(const FObjectInitializer& ObjectInitializer);

public:
	/** 위젯을 페이드 인하여 표시하는 함수입니다. */
	UFUNCTION()
	void ShowWidget();

	/** 위젯을 페이드 아웃하여 숨기는 함수입니다. */
	UFUNCTION()
	void HideWidget();

private:
	/** 페이드 타이머를 업데이트하는 함수입니다. */
	void UpdateFadeTimer();

	/** 페이드 타이머를 시작하는 함수입니다. */
	void StartFadeTimer();

	/** 페이드 타이머를 정지하는 함수입니다. */
	void StopFadeTimer();
	
protected:
	/** 페이드 애니메이션 시간입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float FadeDuration;

private:
	/** 페이드 타이머 핸들입니다. */
	FTimerHandle FadeTimerHandle;

	/** 현재 투명도 값입니다. */
	float CurrentOpacity;

	/** 목표 투명도 값입니다. */
	float TargetOpacity;

	/** 현재 페이드 진행 시간입니다. */
	float CurrentFadeTime;

	/** 페이드 시작 시점의 투명도 값입니다. */
	float StartOpacity;

	/** 페이드 업데이트 간격입니다. */
	float FadeUpdateRate;
};