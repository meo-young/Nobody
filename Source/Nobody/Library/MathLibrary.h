#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathLibrary.generated.h"

UCLASS()
class NOBODY_API UMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** 인자의 유효한 범위 내의 랜덤 숫자를 반환하는 함수입니다. */
	UFUNCTION(BlueprintCallable)
	static float GetRandomInRange(float InValue);
	
	
};
