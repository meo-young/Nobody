#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Footstep.generated.h"

UCLASS()
class NOBODY_API UAnimNotify_Footstep : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void PostInitProperties() override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
private:
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	
};
