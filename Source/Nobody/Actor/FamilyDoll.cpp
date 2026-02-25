#include "Actor/FamilyDoll.h"
#include "Character/Enemy/EnemyBase.h"
#include "Components/BoxComponent.h"
#include "Define/Define.h"
#include "Enum/EInteractType.h"
#include "Library/SoundLibrary.h"
#include "Enum/ESFX.h"
#include "Sound/SoundCue.h"

AFamilyDoll::AFamilyDoll()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(Root);
	
	DollMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DollMesh"));
	DollMesh->SetupAttachment(RootComponent);
	
	InteractionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZone"));
	InteractionZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionZone->SetCollisionResponseToChannel(ECC_INTERACTION, ECR_Block);
	InteractionZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	InteractionZone->SetupAttachment(Root);
	
	InteractionType = EInteractionType::Sound;
}

void AFamilyDoll::Interact_Implementation()
{
	IInteractable::Interact_Implementation();
	
	// 상호작용 콜리전을 비활성화합니다.
	InteractionZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 이벤트가 활성화된 상태면 Noise를 재생하고, 아니면 인형 소리를 재생합니다.
	if (TargetEnemy && TargetEnemy->bIsEventActive)
	{
		PlayNoiseSound();
	}
	else
	{
		PlayDollSound();
	}
}

EInteractionType AFamilyDoll::GetInteractionType()
{
	return InteractionType;
}

void AFamilyDoll::PlayDollSound()
{
	if (DollSounds.IsEmpty())
	{
		return;
	}
	
	// 배열에서 랜덤으로 소리를 선택합니다.
	const int32 RandomIndex = FMath::RandRange(0, DollSounds.Num() - 1);
	USoundCue* SelectedSound = DollSounds[RandomIndex];
	
	// 소리의 길이만큼 대기한 후 콜리전을 활성화합니다.
	if (SelectedSound)
	{
		const float SoundDuration = SelectedSound->GetDuration();
		GetWorldTimerManager().SetTimer(CollisionTimerHandle, this, &AFamilyDoll::ActivateCollision, SoundDuration, false);
	}
	
	// 선택한 소리를 재생한 후 배열에서 제거합니다.
	USoundLibrary::PlaySFXInLocation(this, SelectedSound, GetActorLocation());
	//DollSounds.RemoveAt(RandomIndex);
}

void AFamilyDoll::PlayNoiseSound()
{
	// 노이즈 소리를 재생합니다.
	USoundLibrary::PlaySFXInLocation(this,NoiseSound, GetActorLocation());
	
	// 소리의 길이만큼 대기한 후 콜리전을 활성화합니다.
	if (NoiseSound)
	{
		const float SoundDuration = NoiseSound->GetDuration();
		GetWorldTimerManager().SetTimer(CollisionTimerHandle, this, &AFamilyDoll::ActivateCollision, SoundDuration, false);
	}
}

void AFamilyDoll::ActivateCollision()
{
	// 상호작용 콜리전을 다시 활성화합니다.
	if (InteractionZone)
	{
		InteractionZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}
