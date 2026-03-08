#include "Pill.h"

#include "Nobody.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/TitleWidget.h"

APill::APill()
{
    PrimaryActorTick.bCanEverTick = false;

    PillMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillMesh"));
    RootComponent = PillMesh;
}

void APill::BeginPlay()
{
    Super::BeginPlay();
    
    /*// 마우스 이벤트 바인딩
    OnBeginCursorOver.AddDynamic(this, &APill::OnBeginCursorOverFunction);
    OnEndCursorOver.AddDynamic(this, &APill::OnEndCursorOverFunction);
    OnClicked.AddDynamic(this, &APill::OnClickedFunction);
    
    if (!LabelWidgetInstance && LabelWidgetClass)
    {
        LabelWidgetInstance = CreateWidget<UTitleWidget>(
            UGameplayStatics::GetPlayerController(this, 0),
            LabelWidgetClass
        );
        
        LabelWidgetInstance->HideWidget();
    }*/
}

void APill::OnBeginCursorOverFunction(AActor* TouchedActor)
{
    ShowLabel(true);
    LOG(TEXT("마우스 호버"))
}

void APill::OnEndCursorOverFunction(AActor* TouchedActor)
{
    ShowLabel(false);
}

void APill::OnClickedFunction(AActor* TouchedActor, FKey ButtonPressed)
{
    ExecutePillAction();
}

void APill::ExecutePillAction()
{
    switch (PillType)
    {
    case EPillType::Start:
        // 게임 레벨로 전환합니다.
        UGameplayStatics::OpenLevel(this, FName("LV_InGame"));
        break;

    case EPillType::Setting:
        // 설정 화면을 여는 로직을 구현합니다.
        UE_LOG(LogTemp, Log, TEXT("설정 화면 열기"));
        break;

    case EPillType::Quit:
        // 게임을 종료합니다.
        UKismetSystemLibrary::QuitGame(
            this,
            UGameplayStatics::GetPlayerController(this, 0),
            EQuitPreference::Quit,
            true
        );
        break;
    }
}

void APill::ShowLabel(bool bShow)
{
    if (bShow)
    {
        if (LabelWidgetInstance)
        {
            LabelWidgetInstance->SetText(LabelText.ToString());
        }
    }
    else
    {
        if (LabelWidgetInstance && LabelWidgetInstance->IsInViewport())
        {
            LabelWidgetInstance->HideWidget();
        }
    }
}