#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/EnemyBase.h"
#include "Doll.generated.h"

UCLASS()
class NOBODY_API ADoll : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	virtual void StartStepSystem() override;
	virtual void StopStepSystem() override;
	
};
