#include "Interaction/InteractionBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Nobody.h"
#include "Camera/CameraComponent.h"
#include "Character/Enemy/EnemyBase.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Define/Define.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerBase.h"

AInteractionBase::AInteractionBase()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Look(TEXT("/Game/_Nobody/Input/IA_Look"));
	if (IA_Look.Succeeded())
	{
		LookAction = IA_Look.Object;	
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Control(TEXT("/Game/_Nobody/Input/IA_Control"));
	if (IA_Control.Succeeded())
	{
		ControlAction = IA_Control.Object;
	}
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZone"));
	InteractionZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionZone->SetCollisionResponseToChannel(ECC_INTERACTION, ECR_Block);
	InteractionZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	InteractionZone->SetupAttachment(Root);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(Root);
}

void AInteractionBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (LookAction)
		{
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::DoLook);
		}

		if (ControlAction)
		{
			EIC->BindAction(ControlAction, ETriggerEvent::Started, this, &ThisClass::DoControl);
		}
	}
}

void AInteractionBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이어의 목표 위치를 초기화합니다.
	PlayerTargetLocation = CameraComponent->GetComponentLocation();
	PlayerTargetLocation += LerpLocationValue;
	PlayerTargetLocation.Z = 342.150107f;
	
	// 플레이어의 목표 회전 값을 초기화합니다.
	PlayerTargetRotation = GetActorRotation();
	PlayerTargetRotation.Yaw = -PlayerTargetRotation.Yaw;
	PlayerTargetRotation += LerpRotationValue;
	
	// 플레이어와 컨트롤러를 캐싱합니다.
	PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AInteractionBase::Interact_Implementation()
{
	IInteractable::Interact_Implementation();
	
	// 상호작용 가능 상태를 초기화합니다.
	bIsInteractPossible = false;
	
	// 상호작용 콜리전을 비활성화합니다.
	InteractionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 액터 시퀀스를 재생합니다.
	PlayInteractionStartSequence();

	// 액터 시퀀스가 재생하는 동안 플레이어 캐릭터를 숨깁니다.
	Player->SetActorHiddenInGame(true);
	Player->GetMesh()->SetEnableAnimation(false);
	
	// 카메라를 보간하고, 액터 시퀀스가 재생하는 동안 입력을 비활성화합니다.
	PlayerController->SetInputEnable(false);
	PlayerController->SetViewTargetWithBlend(this,0.5f);
}

void AInteractionBase::OnStartActorSequenceEnded()
{
	// 플레이어 컨트롤러를 빙의시킵니다.
	PlayerController->Possess(this);
	PlayerController->SetInputEnable(true);
	
	// 카메라의 초기위치를 저장합니다.
	OriginRotation = CameraComponent->GetRelativeRotation();
	CurrentYawOffset = 0.f;
	CurrentPitchOffset = 0.f;
}

void AInteractionBase::OnEndActorSequenceEnded()
{
	// 플레이어 회전 값을 초기화하고, 입력이 활성화된 상태로 빙의합니다.
	PlayerController->Possess(Player);
	PlayerController->SetControlRotation(PlayerTargetRotation);
	PlayerController->SetInputEnable(true);
	
	// 플레이어의 모습을 활성화합니다.
	Player->GetMesh()->SetEnableAnimation(true);
	Player->SetActorHiddenInGame(false);
	
	// 상호작용 콜리전을 활성화합니다.
	InteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AInteractionBase::CheckIfEventActivated()
{
	if (bIsEventActivated)
	{
		LOG(TEXT("이벤트가 활성화된 상태입니다. 이벤트를 초기화합니다."))
		EventEnemy->PauseStepSystem();	
		GetWorldTimerManager().SetTimer(InitHandle, this, &ThisClass::InitEvent, 1.0f, false);
	}
	else
	{
		LOG(TEXT("이벤트가 활성화되지 않은 상태입니다"))
		GetWorldTimerManager().SetTimer(InitHandle, this, &ThisClass::InitEvent, 0.5f, false);
	}
}

void AInteractionBase::InitEvent()
{
	if (bIsEventActivated)
	{
		EventEnemy->StopStepSystem();
	}
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		// 상호작용 상태를 활성화합니다.
		bIsInteractPossible = true;
	}), 0.5f, false);
}

void AInteractionBase::DoLook(const FInputActionValue& Value)
{
	if (!bIsInteractPossible) return;
	
	// 마우스 입력으로부터 FVector2D 값을 추출합니다.
	const FVector2D LookInput = Value.Get<FVector2D>();
	const float YawInput = LookInput.X;
	const float PitchInput = LookInput.Y;

	// 초기 위치로부터 최대 MaxAngle만큼 회전 값을 누적시킵니다.
	CurrentYawOffset += YawInput;
	CurrentPitchOffset += PitchInput;
	CurrentYawOffset = FMath::Clamp(CurrentYawOffset, -MaxYawAngle, MaxYawAngle);
	CurrentPitchOffset = FMath::Clamp(CurrentPitchOffset, -MaxPitchAngle, MaxPitchAngle);

	// 초기 위치에 누적한 회전값을 적용합니다.
	FRotator NewRot = OriginRotation;
	NewRot.Yaw += CurrentYawOffset;
	NewRot.Pitch -= CurrentPitchOffset;
	
	// 최종 회전값을 적용합니다.
	CameraComponent->SetRelativeRotation(NewRot);
}

void AInteractionBase::DoControl(const FInputActionValue& Value)
{
	if (!bIsInteractPossible) return;

	// 상호작용 종료 시퀀스를 재생합니다.
	PlayInteractionEndSequence();
	
	// 플레이어의 위치, 회전 값을 초기화합니다.
	Player->SetActorLocation(PlayerTargetLocation);
	Player->SetActorRotation(PlayerTargetRotation);
	Player->GetCameraComponent()->SetWorldRotation(PlayerTargetRotation);
	
	// 입력을 비활성화하고, 카메라를 보간합니다.
	PlayerController->SetInputEnable(false);
	PlayerController->SetViewTargetWithBlend(Player, 1.3f);
	
	// 상호작용 가능 상태를 초기화합니다.
	bIsInteractPossible = false;
}