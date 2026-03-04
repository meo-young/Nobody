#include "PlayerCharacter.h"
#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Nobody.h"
#include "InputAction.h"
#include "SkeletalMeshMerge.h"
#include "Camera/CameraComponent.h"
#include "Component/EffectComponent.h"
#include "Component/FootstepComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/InteractionComponent.h"
#include "Component/VoiceComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerController/PlayerControllerBase.h"

APlayerCharacter::APlayerCharacter()
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = 100.0f;
	CharacterMovementComponent->MaxStepHeight = 15.0f;
	
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	FootstepComponent = CreateDefaultSubobject<UFootstepComponent>(TEXT("Footstep Component"));
	EffectComponent = CreateDefaultSubobject<UEffectComponent>(TEXT("Effect Component"));
	VoiceComponent = CreateDefaultSubobject<UVoiceComponent>(TEXT("Voice Component"));
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	SpringArmComponent->SetupAttachment(GetMesh(), FName("head"));
	SpringArmComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	SpringArmComponent->CameraRotationLagSpeed = 17.0f;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->TargetArmLength = 0.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->bEnableFirstPersonFieldOfView = true;
	CameraComponent->bEnableFirstPersonScale = true;
	CameraComponent->FirstPersonFieldOfView = 70.0f;
	CameraComponent->FirstPersonScale = 0.6f;
	
	bUseControllerRotationYaw = false;
	
	DollJumpScareMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Doll JumpScare Mesh"));
	DollJumpScareMesh->SetupAttachment(CameraComponent);

	DollClothMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Doll Cloth Mesh"));
	DollClothMesh->SetupAttachment(DollJumpScareMesh);
	
	MannequinJumpScareMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mannequin JumpScare Mesh"));
	MannequinJumpScareMesh->SetupAttachment(CameraComponent);

	JumpScareLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("JumpScare Light"));
	JumpScareLight->SetupAttachment(CameraComponent);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::DoMove);
		}
		
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::DoLook);
		}
		
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::DoInteract);
		}
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerControllerBase>(GetController());
	
	MergeAndApplyMeshes();
	InitCharacterSetting();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// 캐릭터 Yaw를 카메라 Lag 속도에 맞춰 보간
	const FRotator TargetRotation = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaSeconds, RotationLagSpeed);
	SetActorRotation(NewRotation);

	
	// 플레이어가 걷는 중이면 발걸음 소리를 재생합니다.
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!IsValid(MovementComponent)) return;
	
	// 땅에 있는지 확인
	const bool bIsOnGround = MovementComponent->IsMovingOnGround();
    
	// 현재 속도 확인 (2D 평면 속도)
	const float CurrentSpeed = MovementComponent->Velocity.Size2D();
    
	// 걷는 중인지 판단
	const bool bIsWalking = bIsOnGround && CurrentSpeed >= KINDA_SMALL_NUMBER;
    
	if (bIsWalking)
	{
		FootstepComponent->StartFootstep();
	}
	else
	{
		FootstepComponent->StopFootstep();
	}
}

void APlayerCharacter::SetEffectEnable(const bool bEnable)
{
	EffectComponent->SetEffectEnable(bEnable);	
}

void APlayerCharacter::ExecuteDeathSequence(EJumpScareType JumpScareType)
{
	ShowDeadJumpScare(JumpScareType);
	ShowDeadJumpScare_Implementation(JumpScareType);
}

void APlayerCharacter::ShowDeadJumpScare_Implementation(EJumpScareType JumpScareType)
{
	switch (JumpScareType)
	{
		case EJumpScareType::Doll:
			DollJumpScareMesh->SetVisibility(true);
			DollClothMesh->SetVisibility(true);
			break;
		
		case EJumpScareType::Mannequin:
			MannequinJumpScareMesh->SetVisibility(true);
			break;
	}

	JumpScareLight->SetVisibility(true);
	PlayerController->SetInputEnable(false);
	SetActorLocation(FVector(-1309.795158f, 2436.616586f, 219.575262f));
}

void APlayerCharacter::DoMove(const FInputActionValue& InputActionValue)
{
	const FVector2D MoveValue = InputActionValue.Get<FVector2D>();
	
	// 카메라/컨트롤러 방향의 Yaw만 추출합니다.
	const FRotator ControlRot = Controller->GetControlRotation();
	const FRotator YawRot(0.f, ControlRot.Yaw, 0.f);

	// 방향 벡터 계산
	const FVector Forward = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	const FVector Right   = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	// 캐릭터를 전방으로 이동시킵니다.
	if (!FMath::IsNearlyZero(MoveValue.X))
	{
		AddMovementInput(Forward, MoveValue.X);
	}

	// 캐릭터를 측면으로 이동시킵니다.
	if (!FMath::IsNearlyZero(MoveValue.Y))
	{
		AddMovementInput(Right, MoveValue.Y);
	}
}

void APlayerCharacter::DoLook(const FInputActionValue& InputActionValue)
{
	const FVector2D LookValue = InputActionValue.Get<FVector2D>();
	
	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}

void APlayerCharacter::DoInteract(const FInputActionValue& InputActionValue)
{
	InteractionComponent->ExecuteInteractIfPossible();
}

void APlayerCharacter::InitCharacterSetting()
{
	MannequinJumpScareMesh->SetVisibility(false);
	JumpScareLight->SetVisibility(false);
	DollJumpScareMesh->SetVisibility(false);
	DollClothMesh->SetVisibility(false);
	PlayerController->SetInputEnable(true);
}

void APlayerCharacter::MergeAndApplyMeshes()
{
	if (DollMeshesToMerge.IsEmpty()) return;

	USkeletalMesh* MergedMesh = NewObject<USkeletalMesh>(
		GetTransientPackage(), NAME_None, RF_Transient
	);

	TArray<FSkelMeshMergeSectionMapping> EmptySectionMappings;
	FSkeletalMeshMerge Merger(MergedMesh, DollMeshesToMerge, EmptySectionMappings, 0);

	if (Merger.DoMerge())
	{
		// 병합 결과는 DollClothMesh에 적용, DollJumpScareMesh(SKM_Body)는 건드리지 않음
		DollClothMesh->SetSkeletalMesh(MergedMesh);
		// SKM_Body의 포즈를 따라감
		DollClothMesh->SetLeaderPoseComponent(DollJumpScareMesh);
	}
	else
	{
		LOG(TEXT("Skeletal Mesh 병합 실패"));
	}
}
