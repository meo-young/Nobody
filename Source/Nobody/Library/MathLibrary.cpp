#include "Library/MathLibrary.h"
#include "Nobody.h"

float UMathLibrary::GetRandomInRange(float InValue)
{
	// 변동 범위를 계산합니다 (BaseValue의 1/3)
	const float Variation = InValue * (1.0f / 3.0f);
    
	// 최소값과 최대값을 계산합니다
	const float MinValue = InValue - Variation;
	const float MaxValue = InValue + Variation;
    
	// 범위 내의 랜덤한 float 값을 반환합니다
	const float RandomValue = FMath::FRandRange(MinValue, MaxValue);
	
	LOG(TEXT("%f"), RandomValue);
	return RandomValue;
}
