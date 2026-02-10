#include "UI/Manual/ManualWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UManualWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 기본값 설정
	if (MouseImageSize.IsZero())
	{
		MouseImageSize = FVector2D(50.0f, 50.0f);
	}

	if (FontSize == 0)
	{
		FontSize = 24;
	}

	if (TextColor == FLinearColor(0, 0, 0, 0))
	{
		TextColor = FLinearColor::White;
	}

	if (ImageTextSpacing == 0.0f)
	{
		ImageTextSpacing = 10.0f;
	}

	if (ContainerSpacing == 0.0f)
	{
		ContainerSpacing = 30.0f;
	}

	// 초기 투명도 설정
	SetRenderOpacity(0.0f);
}

void UManualWidget::SetLeftClickAction(const FString& ActionText)
{
	// 기존 좌클릭 컨테이너가 있다면 제거
	ClearLeftClickAction();

	// 새로운 좌클릭 컨테이너 생성 및 추가
	CreateLeftClickContainer(ActionText);
}

void UManualWidget::SetRightClickAction(const FString& ActionText)
{
	// 기존 우클릭 컨테이너가 있다면 제거
	ClearRightClickAction();

	// 새로운 우클릭 컨테이너 생성 및 추가
	CreateRightClickContainer(ActionText);
}

void UManualWidget::ClearAllActions()
{
	if (MainContainer)
	{
		MainContainer->ClearChildren();
		LeftClickContainer = nullptr;
		RightClickContainer = nullptr;
	}
}

void UManualWidget::ClearLeftClickAction()
{
	if (LeftClickContainer && MainContainer)
	{
		MainContainer->RemoveChild(LeftClickContainer);
		LeftClickContainer = nullptr;
	}
}

void UManualWidget::ClearRightClickAction()
{
	if (RightClickContainer && MainContainer)
	{
		MainContainer->RemoveChild(RightClickContainer);
		RightClickContainer = nullptr;
	}
}

void UManualWidget::CreateLeftClickContainer(const FString& ActionText)
{
	if (!MainContainer)
	{
		return;
	}

	// 좌클릭용 HorizontalBox 생성
	LeftClickContainer = NewObject<UHorizontalBox>(this);
	if (!LeftClickContainer)
	{
		return;
	}

	// 마우스 이미지 생성 및 추가
	UImage* MouseImage = CreateMouseImage(true);
	if (MouseImage)
	{
		UHorizontalBoxSlot* ImageSlot = Cast<UHorizontalBoxSlot>(LeftClickContainer->AddChild(MouseImage));
		if (ImageSlot)
		{
			// 이미지 수직 정렬 설정
			ImageSlot->SetVerticalAlignment(VAlign_Center);
		}
	}

	// 액션 텍스트 생성 및 추가
	UTextBlock* ActionTextBlock = CreateActionText(ActionText);
	if (ActionTextBlock)
	{
		UHorizontalBoxSlot* TextSlot = Cast<UHorizontalBoxSlot>(LeftClickContainer->AddChild(ActionTextBlock));
		if (TextSlot)
		{
			// 이미지와 텍스트 사이 간격 설정
			TextSlot->SetPadding(FMargin(ImageTextSpacing, 0, 0, 0));
			// 텍스트 수직 정렬 설정
			TextSlot->SetVerticalAlignment(VAlign_Center);
		}
	}

	// MainContainer에 추가
	UHorizontalBoxSlot* ContainerSlot = Cast<UHorizontalBoxSlot>(MainContainer->AddChild(LeftClickContainer));
	if (ContainerSlot)
	{
		// 우클릭 컨테이너가 있다면 간격 추가
		if (RightClickContainer)
		{
			ContainerSlot->SetPadding(FMargin(0, 0, ContainerSpacing, 0));
		}
		// 컨테이너 수직 정렬 설정
		ContainerSlot->SetVerticalAlignment(VAlign_Center);
	}
}

void UManualWidget::CreateRightClickContainer(const FString& ActionText)
{
	if (!MainContainer)
	{
		return;
	}

	// 우클릭용 HorizontalBox 생성
	RightClickContainer = NewObject<UHorizontalBox>(this);
	if (!RightClickContainer)
	{
		return;
	}

	// 마우스 이미지 생성 및 추가
	UImage* MouseImage = CreateMouseImage(false);
	if (MouseImage)
	{
		UHorizontalBoxSlot* ImageSlot = Cast<UHorizontalBoxSlot>(RightClickContainer->AddChild(MouseImage));
		if (ImageSlot)
		{
			// 이미지 수직 정렬 설정
			ImageSlot->SetVerticalAlignment(VAlign_Center);
		}
	}

	// 액션 텍스트 생성 및 추가
	UTextBlock* ActionTextBlock = CreateActionText(ActionText);
	if (ActionTextBlock)
	{
		UHorizontalBoxSlot* TextSlot = Cast<UHorizontalBoxSlot>(RightClickContainer->AddChild(ActionTextBlock));
		if (TextSlot)
		{
			// 이미지와 텍스트 사이 간격 설정
			TextSlot->SetPadding(FMargin(ImageTextSpacing, 0, 0, 0));
			// 텍스트 수직 정렬 설정
			TextSlot->SetVerticalAlignment(VAlign_Center);
		}
	}

	// MainContainer에 추가
	UHorizontalBoxSlot* ContainerSlot = Cast<UHorizontalBoxSlot>(MainContainer->AddChild(RightClickContainer));
	if (ContainerSlot)
	{
		// 컨테이너 수직 정렬 설정
		ContainerSlot->SetVerticalAlignment(VAlign_Center);
	}
}

UImage* UManualWidget::CreateMouseImage(bool bIsLeftClick)
{
	// 이미지 위젯 생성
	UImage* MouseImage = NewObject<UImage>(this);
	if (!MouseImage)
	{
		return nullptr;
	}

	// 적절한 텍스처 설정
	UTexture2D* TargetTexture = bIsLeftClick ? LeftClickTexture : RightClickTexture;
	if (TargetTexture)
	{
		MouseImage->SetBrushFromTexture(TargetTexture);
		MouseImage->SetDesiredSizeOverride(MouseImageSize);
	}

	return MouseImage;
}

UTextBlock* UManualWidget::CreateActionText(const FString& ActionText)
{
	// 텍스트 블록 생성
	UTextBlock* TextBlock = NewObject<UTextBlock>(this);
	if (!TextBlock)
	{
		return nullptr;
	}

	// 텍스트 설정
	TextBlock->SetText(FText::FromString(ActionText));
	
	// 폰트 설정
	FSlateFontInfo FontInfo = TextBlock->GetFont();
	
	// 커스텀 폰트가 설정되어 있다면 적용
	if (TextFont)
	{
		FontInfo.FontObject = TextFont;
	}
	
	FontInfo.Size = FontSize;
	TextBlock->SetFont(FontInfo);

	// 색상 설정
	TextBlock->SetColorAndOpacity(FSlateColor(TextColor));

	return TextBlock;
}