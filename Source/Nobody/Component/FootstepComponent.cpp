#include "Component/FootstepComponent.h"
#include "Nobody.h"
#include "Define/Define.h"
#include "Enum/ESFX.h"
#include "Library/SoundLibrary.h"

UFootstepComponent::UFootstepComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFootstepComponent::BeginPlay()
{
	Super::BeginPlay();
	
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.bReturnPhysicalMaterial = true;
}

void UFootstepComponent::EndPlay(const EEndPlayReason::Type EndReason)
{
	Super::EndPlay(EndReason);
	
	// 컴포넌트가 종료될 때 타이머를 정리합니다.
	StopFootstep();
}

void UFootstepComponent::StartFootstep()
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	UWorld* World = Owner->GetWorld();
	if (!IsValid(World)) return;

	// 이미 타이머가 실행 중이면 중복 실행 방지
	if (World->GetTimerManager().IsTimerActive(FootstepTimerHandle))
	{
		return;
	}
	
	// 0.5초마다 PlayFootstepSound 함수를 반복 호출
	World->GetTimerManager().SetTimer(
		FootstepTimerHandle,
		this,
		&ThisClass::PlayFootstepSound,
		FootstepInterval,
		true
	);
}

void UFootstepComponent::StopFootstep()
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	UWorld* World = Owner->GetWorld();
	if (!IsValid(World)) return;

	// 타이머 정지
	World->GetTimerManager().ClearTimer(FootstepTimerHandle);
}

void UFootstepComponent::PlayFootstepSound()
{
	const AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	const UWorld* World = Owner->GetWorld();
	if (!IsValid(World)) return;

	// 액터의 현재 위치에서 아래 방향으로 LineTrace 실시
	const FVector CurrentLocation = Owner->GetActorLocation();
	const FVector DownVector = Owner->GetActorUpVector() * -TraceDistance;
	const FVector TargetLocation = CurrentLocation + DownVector;
	
	// LineTrace 실시
	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		CurrentLocation,
		TargetLocation,
		ECC_FOOTSTEP,
		QueryParams
	);

#if WITH_EDITOR
	// 에디터에서 디버그 라인 그리기 (필요시 주석 해제)
	// DrawDebugLine(World, CurrentLocation, TargetLocation, FColor::Green, false, 1.0f, 0, 2.0f);
#endif

	// Surface Type에 따른 발걸음 소리 재생
	if (bHit)
	{
		if (HitResult.PhysMaterial.IsValid())
		{
			switch (HitResult.PhysMaterial->SurfaceType)
			{
			case SURFACE_RUG:
				USoundLibrary::PlaySFXInLocation(Owner, ESFX::Footstep_Rug, HitResult.ImpactPoint - FVector(0.0f, 0.0f, 100.0f));
				break;

			default:
				USoundLibrary::PlaySFXInLocation(Owner, ESFX::Footstep_Rug, HitResult.ImpactPoint - FVector(0.0f, 0.0f, 100.0f));
				break;
			}
		}
	}
}
