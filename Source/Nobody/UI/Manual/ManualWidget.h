#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Engine/Font.h"
#include "ManualWidget.generated.h"

class UHorizontalBox;
class UImage;
class UTextBlock;

/**
 * 상호작용 가능한 오브젝트의 조작 방법을 표시하는 UI 위젯 클래스입니다.
 */
UCLASS()
class NOBODY_API UManualWidget : public UWidgetBase
{
	GENERATED_BODY()

public:
	/** 위젯 초기화 함수입니다. */
	virtual void NativeConstruct() override;

public:
	/** 좌클릭 조작 정보를 설정하는 함수입니다. */
	void SetLeftClickAction(const FString& ActionText);

	/** 우클릭 조작 정보를 설정하는 함수입니다. */
	void SetRightClickAction(const FString& ActionText);

	/** 모든 조작 정보를 초기화하는 함수입니다. */
	void ClearAllActions();

	/** 좌클릭 조작 정보를 제거하는 함수입니다. */
	void ClearLeftClickAction();

	/** 우클릭 조작 정보를 제거하는 함수입니다. */
	void ClearRightClickAction();

private:
	/** 좌클릭 컨테이너를 생성하고 추가하는 함수입니다. */
	void CreateLeftClickContainer(const FString& ActionText);

	/** 우클릭 컨테이너를 생성하고 추가하는 함수입니다. */
	void CreateRightClickContainer(const FString& ActionText);

	/** 마우스 이미지를 생성하는 함수입니다. */
	UImage* CreateMouseImage(bool bIsLeftClick);

	/** 액션 텍스트를 생성하는 함수입니다. */
	UTextBlock* CreateActionText(const FString& ActionText);

protected:
	/** 조작 정보를 담을 메인 컨테이너입니다. */
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* MainContainer;

protected:
	/** 좌클릭 마우스 이미지 텍스처입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	UTexture2D* LeftClickTexture;

	/** 우클릭 마우스 이미지 텍스처입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	UTexture2D* RightClickTexture;

	/** 마우스 이미지 크기입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	FVector2D MouseImageSize;

	/** 텍스트 폰트입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	UFont* TextFont;

	/** 텍스트 폰트 크기입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	int32 FontSize;

	/** 텍스트 색상입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	FLinearColor TextColor;

	/** 이미지와 텍스트 사이 간격입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float ImageTextSpacing;

	/** 좌클릭과 우클릭 컨테이너 사이 간격입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	float ContainerSpacing;

private:
	/** 좌클릭 컨테이너를 참조하는 변수입니다. */
	UPROPERTY()
	UHorizontalBox* LeftClickContainer;

	/** 우클릭 컨테이너를 참조하는 변수입니다. */
	UPROPERTY()
	UHorizontalBox* RightClickContainer;

};