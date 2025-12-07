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
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> BedMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|컴포넌트")
	TObjectPtr<UStaticMeshComponent> PillowMesh;
	
};
