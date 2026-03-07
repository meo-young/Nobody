#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionBase.h"
#include "Bed.generated.h"

UCLASS()
class NOBODY_API ABed : public AInteractionBase
{
	GENERATED_BODY()
	
public:
	ABed();
	virtual void BeginPlay() override;

public:
	virtual void Interact_Implementation() override;

protected:
	virtual void OnEndActorSequenceEnded() override;
	virtual void InitEvent() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> BedMesh;

	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> PillowMesh;

	/** 플레이어가 침대 오른쪽에서 상호작용했는지 나타내는 변수 */
	UPROPERTY(BlueprintReadOnly, Category = "변수")
	bool bIsRightSide = false;

};
