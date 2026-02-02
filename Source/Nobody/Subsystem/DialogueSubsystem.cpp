#include "Subsystem/DialogueSubsystem.h"
#include "TableRowBase/VoiceTableRowBase.h"
#include "Enum/EVoice.h"
#include "Nobody.h"
#include "Library/SoundLibrary.h"
#include "PlayerController/PlayerControllerBase.h"
#include "UI/Dialogue/DialogueWidget.h"

void UDialogueSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	DialogueDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/_Nobody/DataTable/DT_Voice"));
}

void UDialogueSubsystem::Init()
{
	if (APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController()))
	{
		LOG(TEXT("DialogueWidgetInstance 추출 성공"))
		DialogueWidgetInstance = PlayerController->GetDialogueWidget();
	}
	
	
	if (DialogueDataTable)
	{
		LOG(TEXT("Voice TableRows 추출 성공"))
		DialogueDataTable->GetAllRows("Voice", DialogueTableRows);
	}
	
	ShowDialogue(EVoice::Scare);
}

void UDialogueSubsystem::ShowDialogue(EVoice InVoice)
{
	if (FVoiceTableRowBase* Row = GetDialogueTableRow(InVoice))
	{
		// Sound와 DialogueText를 각각 분리하여 저장합니다.
		TSoftObjectPtr<USoundCue> FoundSound = Row->Sound;
		FString FoundDialogueText = Row->DialogueText;
		
		// 대사의 길이만큼 대사를 화면에 출력하고, 음성을 재생합니다.
		USoundLibrary::PlaySFX2D(GetWorld(), FoundSound.LoadSynchronous());
		const float SoundLength = FoundSound->GetDuration();
		DialogueWidgetInstance->SetText(FoundDialogueText, SoundLength);
	}
	else
	{
		LOG(TEXT("경고: 해당하는 Voice를 찾을 수 없습니다."));
	}
}

void UDialogueSubsystem::ShowDialogue(EVoice InVoice, const FVector& InLocation)
{
	if (FVoiceTableRowBase* Row = GetDialogueTableRow(InVoice))
	{
		// Sound와 DialogueText를 각각 분리하여 저장합니다.
		TSoftObjectPtr<USoundCue> FoundSound = Row->Sound;
		FString FoundDialogueText = Row->DialogueText;
		
		// 대사의 길이만큼 대사를 화면에 출력하고, 음성을 재생합니다.
		USoundLibrary::PlaySFXInLocation(GetWorld(), FoundSound.LoadSynchronous(), InLocation);
		const float SoundLength = FoundSound->GetDuration();
		DialogueWidgetInstance->SetText(FoundDialogueText, SoundLength);
	}
	else
	{
		LOG(TEXT("경고: 해당하는 Voice를 찾을 수 없습니다."));
	}
}

FVoiceTableRowBase* UDialogueSubsystem::GetDialogueTableRow(EVoice InVoice) const
{
	// Enum 값을 FName으로 직접 변합니다.
	FString EnumValueString = UEnum::GetValueAsString(InVoice);
	
	// EVoice::Scare 형태에서 "Scare" 부분만 추출합니다.
	FString EnumName;
	EnumValueString.Split(TEXT("::"), nullptr, &EnumName);
	FName RowName = FName(*EnumName);
	
	// 해당하는 행을 데이터테이블에서 찾습니다.
	FVoiceTableRowBase* Row = DialogueDataTable->FindRow<FVoiceTableRowBase>(RowName, TEXT("ShowDialogue"));
	
	return Row;
}
