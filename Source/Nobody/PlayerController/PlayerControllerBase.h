#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UFadeWidget;
class UManualWidget;
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
	UFUNCTION(BlueprintCallable)
	void SetInputEnable(const bool InEnable);
	
	void CreateWidgetInstance();
	
	void RemoveWidgetInstance();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputMappingContext> MappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCrosshairWidget> CrosshairWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UDialogueWidget> DialogueWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDialogueWidget> DialogueWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UManualWidget> ManualWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UManualWidget> ManualWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UFadeWidget> FadeWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UFadeWidget> FadeWidgetInstance;
	
private:
	uint8 bActivated : 1 = false;
	
public:
	FORCEINLINE UCrosshairWidget* GetCrosshairWidget() const { return CrosshairWidgetInstance; }
	FORCEINLINE UDialogueWidget* GetDialogueWidget() const { return DialogueWidgetInstance; }
	FORCEINLINE UManualWidget* GetManualWidget() const { return ManualWidgetInstance; }
	FORCEINLINE UFadeWidget* GetFadeWidget() const { return FadeWidgetInstance; }	
};
