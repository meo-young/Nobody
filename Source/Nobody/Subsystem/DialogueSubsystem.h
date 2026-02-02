#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueSubsystem.generated.h"

class UDialogueWidget;
enum class EVoice : uint8;
struct FVoiceTableRowBase;

UCLASS()
class NOBODY_API UDialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
public:
	void Init();
	void ShowDialogue(EVoice InVoice);
	void ShowDialogue(EVoice InVoice, const FVector& InLocation);
	
private:
	FVoiceTableRowBase* GetDialogueTableRow(EVoice InVoice) const;
	
private:
	/** DialogueDataTable에 대한 참조 변수입니다. Initialize 함수에서 초기화됩니다. */
	UPROPERTY()
	TObjectPtr<UDataTable> DialogueDataTable;
	
	/** 대화 위젯 인스턴스에 대한 참조 변수입니다. */
	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidgetInstance;
	
	/** DialogueDataTable의 모든 행에 대한 포인터 배열입니다. Init 함수에서 초기화됩니다. */
	TArray<FVoiceTableRowBase*> DialogueTableRows;
	
};
