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
	
	/** 배열에서 랜덤한 요소를 반환하고 해당 요소를 배열에서 제거하는 함수입니다. */
	template<typename T>
	static T GetRandomAndRemove(TArray<T>& InArray);
};

template <typename T>
T UMathLibrary::GetRandomAndRemove(TArray<T>& InArray)
{
	// 배열이 비어있는지 확인합니다.
	if (InArray.Num() == 0)
	{
		return T();
	}
    
	// 랜덤한 인덱스를 생성합니다.
	const int32 RandomIndex = FMath::RandRange(0, InArray.Num() - 1);
    
	// 해당 인덱스의 요소를 가져옵니다.
	T RandomElement = InArray[RandomIndex];
    
	// 배열에서 해당 요소를 제거합니다.
	InArray.RemoveAt(RandomIndex);
    
	return RandomElement;
}
