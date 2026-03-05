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
	
	/** 옷장 문이 열릴 때 이동할 속도를 저장해 놓은 변수입니다. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "변수")
	float MoveSpeed = 50.0f;
	
private:
	/** 옷장 문이 열리기 전의 위치를 저장해 놓은 변수입니다. */
	FVector OriginalLocation;
	
	/** 옷장 문이 열릴 때 이동할 위치를 저장해 놓은 변수입니다. */
	FVector TargetLocation;
	
	/** 플레이어에 대한 캐싱 변수입니다. 숨어있는 상태인지 확인하기 위해 사용합니다. */
	UPROPERTY()
	TObjectPtr<APlayerCharacter> Player;

};