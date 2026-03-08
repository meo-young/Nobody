#include "Interaction/InteractionBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Nobody.h"
#include "Camera/CameraComponent.h"
#include "Character/Enemy/EnemyBase.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/InteractionComponent.h"
#include "Component/VoiceComponent.h"
#include "Components/BoxComponent.h"
#include "Define/Define.h"
#include "GameFramework/SpringArmComponent.h"
#include "Enum/EVoiceType.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerBase.h"
#include "UI/Manual/ManualWidget.h"

AInteractionBase::AInteractionBase()
{
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
	
	// 이벤트 스폰 타이머를 일시정지하고 상호작용 중임을 나타냅니다.
	if (EventEnemy)
	{
		EventEnemy->PauseSpawnSystem();
		EventEnemy->bIsInteracting = true;	
	}
}

void AInteractionBase::ForceCancel()
{
	// 진행 중인 타이머를 정리합니다.
	GetWorldTimerManager().ClearTimer(InitHandle);

	// 적 이벤트 상태를 복구합니다.
	if (EventEnemy)
	{
		EventEnemy->ResetRespawnTimer();
		EventEnemy->bIsInteracting = false;
	}
}

EInteractionType AInteractionBase::GetInteractionType()
{
	return InteractionType;
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
	
	// PlayerCharacter의 InteractionComponent Tick을 비활성화합니다.
	if (UInteractionComponent* PlayerInteractionComp = Player->FindComponentByClass<UInteractionComponent>())
	{
		PlayerInteractionComp->SetTickEnabled(false);
	}
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
	
	// 상호작용시 이벤트 활성화까지 남은 시간을 확인하고, 타이머를 재가동합니다.
	if (EventEnemy)
	{
		EventEnemy->ResetRespawnTimer();
		EventEnemy->bIsInteracting = false;	
	}
	
	// PlayerCharacter의 InteractionComponent Tick을 다시 활성화합니다.
	if (UInteractionComponent* PlayerInteractionComp = Player->FindComponentByClass<UInteractionComponent>())
	{
		PlayerInteractionComp->SetTickEnabled(true);
	}
}

void AInteractionBase::CheckIfEventActivated()
{
	float TimeDelay = 0.0f;
	
	if (bIsEventActivated)
	{
		LOG(TEXT("이벤트가 활성화된 상태입니다. 이벤트를 초기화합니다."))
		EventEnemy->PauseStepSystem();	
		TimeDelay = 2.0f;
	}
	else
	{
		LOG(TEXT("이벤트가 활성화되지 않은 상태입니다"))
		//Player->GetVoiceComponent()->PlayVoiceWithDelay(EVoiceType::EmptyDoor, 0.5f);
		TimeDelay = 0.01f;
	}
	
	GetWorldTimerManager().SetTimer(InitHandle, this, &ThisClass::InitEvent, TimeDelay, false);
}

void AInteractionBase::InitEvent()
{
	if (bIsEventActivated)
	{
		EventEnemy->StopStepSystem();
	}
	
	PlayerController->GetManualWidget()->ClearAllActions();
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		// 상호작용 상태를 활성화합니다.
		bIsInteractPossible = true;
	}), 0.25f, false);
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
	
	PlayerController->GetManualWidget()->HideWidget();

	// 상호작용 종료 시퀀스를 재생합니다.
	PlayInteractionEndSequence();
	
	// 플레이어의 위치, 회전 값을 초기화합니다.
	Player->SetActorLocation(PlayerTargetLocation);
	PlayerController->SetControlRotation(PlayerTargetRotation);
	Player->SetActorRotation(PlayerTargetRotation);
	
	// 입력을 비활성화하고, 카메라를 보간합니다.
	PlayerController->SetInputEnable(false);
	PlayerController->SetViewTargetWithBlend(Player, 1.3f);
	
	// 상호작용 가능 상태를 초기화합니다.
	bIsInteractPossible = false;
}