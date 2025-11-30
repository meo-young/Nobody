#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UCrosshairWidget;
class UInputMappingContext;

UCLASS()
class NOBODY_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlayerControllerBase();
	virtual void SetupInputComponent() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
private:
	void CreateWidgetInstance();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "변수|입력")
	TObjectPtr<UInputMappingContext> MappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "변수|UI")
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UCrosshairWidget> CrosshairWidgetInstance;
	
public:
	FORCEINLINE UCrosshairWidget* GetCrosshairWidget() const { return CrosshairWidgetInstance; }
	
};
