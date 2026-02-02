#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UDialogueWidget;
class UCrosshairWidget;
class UInputMappingContext;

UCLASS()
class NOBODY_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

public:
	void SetInputEnable(const bool InEnable);
	
private:
	void CreateWidgetInstance();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputMappingContext> MappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UCrosshairWidget> CrosshairWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidgetInstance;
	
private:
	uint8 bActivated : 1 = false;
	
public:
	FORCEINLINE UCrosshairWidget* GetCrosshairWidget() const { return CrosshairWidgetInstance; }
	FORCEINLINE UDialogueWidget* GetDialogueWidget() const { return DialogueWidgetInstance; }
	
};
