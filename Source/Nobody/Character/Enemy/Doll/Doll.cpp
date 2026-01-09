#include "Character/Enemy/Doll/Doll.h"
#include "Interaction/InteractionBase.h"

void ADoll::StartStepSystem()
{
	Super::StartStepSystem();
	
	InteractionObject->SetEventActivated(true);
}

void ADoll::StopStepSystem()
{
	Super::StopStepSystem();
	
	InteractionObject->SetEventActivated(false);
}
