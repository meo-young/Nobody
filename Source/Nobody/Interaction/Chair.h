#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionBase.h"
#include "Chair.generated.h"

UCLASS()
class NOBODY_API AChair : public AInteractionBase
{
	GENERATED_BODY()
	
public:
	AChair();
	
protected:
	virtual void InitEvent() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> ChairMesh;
	
};
