#include "Component/VoiceComponent.h"
#include "Enum/EVoiceType.h"
#include "Enum/EVoice.h"
#include "Library/DialogueLibrary.h"
#include "Library/MathLibrary.h"


void UVoiceComponent::PlayVoiceWithDelay(EVoiceType InVoiceType, const float InDelay)
{
	if (bIsEnableVoice == false) return;
	
	// InDelay 후에 음성을 재생합니다.
	bIsEnableVoice = false;
	CurrentVoiceType = InVoiceType;
	GetWorld()->GetTimerManager().SetTimer(VoiceTimerHandle, this, &UVoiceComponent::PlayVoice, InDelay, false);
}

void UVoiceComponent::PlayVoice()
{
	// 음성 재생 후 쿨타임 타이머를 설정합니다.
	GetWorld()->GetTimerManager().SetTimer(VoiceTimerHandle, this, &UVoiceComponent::ResetVoiceTimer, VoiceCooldownTime, false);
	
	switch (CurrentVoiceType)
	{
	case EVoiceType::EmptyDoor:
		UDialogueLibrary::ShowDialogue(GetOwner(), UMathLibrary::GetRandomAndRemove(EmptyDoorVoices));
		break;
	}
}

void UVoiceComponent::ResetVoiceTimer()
{
	// 음성 재생 가능 상태로 변경합니다.
	bIsEnableVoice = true;
}
