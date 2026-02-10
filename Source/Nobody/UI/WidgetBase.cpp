#include "UI/WidgetBase.h"
#include "Nobody.h"

UWidgetBase::UWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 기본값 설정
	FadeDuration = 1.0f;
	FadeUpdateRate = 0.016f; // 약 60fps
	CurrentOpacity = 0.0f;
	TargetOpacity = 0.0f;
	CurrentFadeTime = 0.0f;
	StartOpacity = 0.0f;
}

void UWidgetBase::ShowWidget()
{
	if (!IsInViewport())
	{
		LOG(TEXT("위젯을 표시합니다"))
		AddToViewport();
	}

	// 위젯을 화면에 표시
	SetVisibility(ESlateVisibility::HitTestInvisible);

	// FadeDuration이 0이면 즉시 표시
	if (FMath::IsNearlyZero(FadeDuration))
	{
		CurrentOpacity = 1.0f;
		SetRenderOpacity(CurrentOpacity);
		return;
	}

	// 페이드 인 시작
	StartOpacity = CurrentOpacity;
	TargetOpacity = 1.0f;
	CurrentFadeTime = 0.0f;

	// 페이드 타이머 시작
	StartFadeTimer();
}

void UWidgetBase::HideWidget()
{
	// FadeDuration이 0이면 즉시 숨김
	if (FMath::IsNearlyZero(FadeDuration))
	{
		CurrentOpacity = 0.0f;
		SetRenderOpacity(CurrentOpacity);
		SetVisibility(ESlateVisibility::Collapsed);
		
		if (IsInViewport())
		{
			LOG(TEXT("위젯을 숨깁니다"))
			RemoveFromParent();
		}
		return;
	}

	// 페이드 아웃 시작
	StartOpacity = CurrentOpacity;
	TargetOpacity = 0.0f;
	CurrentFadeTime = 0.0f;

	// 페이드 타이머 시작
	StartFadeTimer();
}

void UWidgetBase::UpdateFadeTimer()
{
	CurrentFadeTime += FadeUpdateRate;
	
	// 현재 투명도에서 목표 투명도까지의 거리를 기준으로 진행 시간 계산
	float OpacityRange = FMath::Abs(TargetOpacity - StartOpacity);
	float AdjustedDuration = FadeDuration * OpacityRange;
	
	float Alpha = FMath::Clamp(CurrentFadeTime / AdjustedDuration, 0.0f, 1.0f);
	CurrentOpacity = FMath::Lerp(StartOpacity, TargetOpacity, Alpha);
	SetRenderOpacity(CurrentOpacity);

	// 페이드 완료 체크
	if (CurrentFadeTime >= AdjustedDuration)
	{
		CurrentOpacity = TargetOpacity;
		SetRenderOpacity(CurrentOpacity);
		StopFadeTimer();
		
		// 페이드 아웃이 완료되었다면 위젯 숨김
		if (FMath::IsNearlyZero(TargetOpacity))
		{
			SetVisibility(ESlateVisibility::Collapsed);
			
			if (IsInViewport())
			{
				LOG(TEXT("위젯을 숨깁니다"))
				RemoveFromParent();
			}
		}
	}
}

void UWidgetBase::StartFadeTimer()
{
	// 기존 타이머가 있다면 정리
	StopFadeTimer();

	// 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		FadeTimerHandle,
		this,
		&UWidgetBase::UpdateFadeTimer,
		FadeUpdateRate,
		true // 반복 실행
	);
}

void UWidgetBase::StopFadeTimer()
{
	// 타이머 정지
	if (FadeTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
		FadeTimerHandle.Invalidate();
	}
}