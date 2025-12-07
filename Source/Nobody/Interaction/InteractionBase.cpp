#include "Interaction/InteractionBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Nobody.h"
#include "Camera/CameraComponent.h"
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
	
	EndSequenceLocation = CameraComponent->GetComponentLocation();
	EndSequenceLocation.Z = 352.150108f;
	EndSequenceLocation += LerpLocationValue;
	EndSequenceRotation = CameraComponent->GetComponentRotation();
}

void AInteractionBase::Interact_Implementation()
{
	IInteractable::Interact_Implementation();
	
	InteractionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player->GetMesh()->SetEnableAnimation(false);

	// 액터 시퀀스를 재생합니다.
	PlayInteractionStartSequence();

	// 액터 시퀀스가 재생하는 동안 플레이어 캐릭터를 숨깁니다.
	Player->SetActorHiddenInGame(true);
	PlayerController->SetInputEnable(false);
	
	// 플레이어 카메라를 SequenceCameraComponent로 전환합니다.
	if (PlayerController)
	{
		PlayerController->SetViewTargetWithBlend(
			this,                   
			0.5f,                   
			VTBlend_Cubic
		);
	}
}

void AInteractionBase::OnStartActorSequenceEnded()
{
	// 플레이어 컨트롤러를 빙의시킵니다.
	PlayerController->Possess(this);
	
	// 카메라의 초기위치를 저장합니다.
	OriginRotation = CameraComponent->GetRelativeRotation();
	OriginRotation += CameraLerpValue;
	
	CameraComponent->SetRelativeRotation(OriginRotation);
	
	CurrentYawOffset = 0.f;
	CurrentPitchOffset = 0.f;

	PlayerController->SetInputEnable(true);
}

void AInteractionBase::OnEndActorSequenceEnded()
{
	// 플레이어 카메라를 SequenceCameraComponent로 전환합니다.
	if (PlayerController)
	{
		PlayerController->SetViewTarget(Player);
		PlayerController->Possess(Player);
		Player->Controller->SetControlRotation(EndSequenceRotation);
		Player->GetMesh()->SetEnableAnimation(true);
	
		PlayerController->SetInputEnable(true);
		InteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		// ActorSequence가 종료된 이후 플레이어의 모습을 다시 활성화 합니다.
		Player->SetActorHiddenInGame(false);
	}
}

void AInteractionBase::DoLook(const FInputActionValue& Value)
{
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
	PlayerController->SetInputEnable(false);

	PlayInteractionEndSequence();
	Player->SetActorLocation(EndSequenceLocation);
}