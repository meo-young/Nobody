#include "Actor/DrawHandle.h"

#include "EnhancedInputComponent.h"
#include "Interaction/Draw.h"
#include "Nobody.h"
#include "Camera/CameraComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Define/Define.h"
#include "Enum/EInteractType.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerController/PlayerControllerBase.h"

ADrawHandle::ADrawHandle()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	HandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle Mesh"));
	HandleMesh->SetupAttachment(Root);
	
	InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZone"));
	InteractionZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionZone->SetCollisionResponseToChannel(ECC_INTERACTION, ECR_Block);
	InteractionZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	InteractionZone->SetupAttachment(HandleMesh);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(Root);
	
	InteractionType = EInteractionType::Inspect;
}

void ADrawHandle::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void ADrawHandle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InteractAction)
		{
			EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::DoInteract);
		}
		
		if (BackAction)
		{
			EIC->BindAction(BackAction, ETriggerEvent::Started, this, &ThisClass::DoBack);
		}
		
		if (LookAction)
		{
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::DoLook);
		}
	}
}

void ADrawHandle::Interact_Implementation()
{
	IInteractable::Interact_Implementation();
	
	InteractionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayInteractionStartSequence();
	PlayerController->SetViewTargetWithBlend(this,0.5f);
}

EInteractionType ADrawHandle::GetInteractionType()
{
	return InteractionType;
}

void ADrawHandle::EnableInteraction()
{
	InteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ADrawHandle::DisableInteraction()
{
	InteractionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADrawHandle::OnStartActorSequenceEnded()
{
	PlayerController->Possess(this);
	PlayerController->SetInputEnable(true);
	
	OriginRotation = CameraComponent->GetRelativeRotation();
	CurrentYawOffset = 0.f;
	CurrentPitchOffset = 0.f;
}

void ADrawHandle::OnEndActorSequenceEnded()
{
	// InteractionZone을 다시 활성화합니다.
	EnableInteraction();

	// 부모인 ADraw로 컨트롤러를 복귀시킵니다.
	if (ADraw* OwnerDraw = Cast<ADraw>(GetAttachParentActor()))
	{
		PlayerController->Possess(OwnerDraw);
		PlayerController->SetInputEnable(true);
	}
}

void ADrawHandle::DoInteract(const FInputActionValue& InputActionValue)
{
	LOG(TEXT("Interaction with Draw Handle"));
	
}

void ADrawHandle::DoLook(const FInputActionValue& InputActionValue)
{
	// 마우스 입력으로부터 FVector2D 값을 추출합니다.
	const FVector2D LookInput = InputActionValue.Get<FVector2D>();
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

void ADrawHandle::DoBack(const FInputActionValue& InputActionValue)
{
	LOG(TEXT("뒤로 가기"));
	PlayInteractionEndSequence();
	
	PlayerController->SetInputEnable(false);
	PlayerController->SetViewTargetWithBlend(GetAttachParentActor(), 0.5f);

}
