#include "Character/Enemy/EnemyBase.h"
#include "Nobody.h"
#include "GameMode/MainGameMode.h"
#include "Interaction/InteractionBase.h"
#include "Kismet/GameplayStatics.h"
#include "Library/MathLibrary.h"
#include "Library/SoundLibrary.h"
#include "Manager/EventSpawnManager.h"
#include "Sound/SoundCue.h"

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	// RespawnDelay 경과 후 스폰 가능한 이벤트 리스트에 추가합니다.
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AEnemyBase::AddToSpawnList, UMathLibrary::GetRandomInRange(RespawnDelay), false);
}

void AEnemyBase::StartStepSystem()
{
	if (bIsInteracting)
	{
		LOG(TEXT("상호작용 중입니다."))
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AEnemyBase::AddToSpawnList, UMathLibrary::GetRandomInRange(RespawnDelay), false);
		return;
	}
	
	CurrentStepIndex = 0;
	MaxStepIndex = EnemyStepInfos.Num();
	
	bIsEventActive = true;
    
	GoToStep(CurrentStepIndex);
    
	GetWorldTimerManager().SetTimer(
	   StepTimerHandle,
	   this,
	   &ThisClass::MoveToNextStep,
	   StepInterval,
	   true
	);
	
	InteractionObject->SetEventActivated(true);
}

void AEnemyBase::PauseStepSystem()
{
	if (EventPausedSound)
	{
		USoundLibrary::PlaySFXInLocation(this, EventPausedSound, EventSpawnLocation);
	}
	
	bIsEventActive = false;
	
	// 스텝 타이머를 중단합니다.
	GetWorldTimerManager().ClearTimer(StepTimerHandle);
}

void AEnemyBase::StopStepSystem()
{
	// 캐릭터 위치를 초기화합니다.
	SetActorTransform(FTransform(
		FRotator(0.0f, 0.0f, 0.0f),
		FVector(0.0f, 0.0f, -500.0f) 
	));
	
	// RespawnDelay 경과 후 스폰 가능한 이벤트 리스트에 추가합니다.
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AEnemyBase::AddToSpawnList, UMathLibrary::GetRandomInRange(RespawnDelay), false);
	
	InteractionObject->SetEventActivated(false);
}

void AEnemyBase::PauseSpawnSystem()
{
	// 이벤트 스폰까지 남은 시간을 받아옵니다.
	RemainingRespawnTime = GetWorldTimerManager().GetTimerRemaining(RespawnTimerHandle);
	GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
	
	LOG(TEXT("이벤트 스폰까지 %f 남았습니다."), RemainingRespawnTime)
}

void AEnemyBase::ResetRespawnTimer()
{
	// 이벤트 스폰까지 남은 시간이 ResetTime보다 클 경우에만 해당 시간으로 타이머를 가동합니다.
	const float ActivatedTimerDelay = FMath::Max(RemainingRespawnTime, ResetTime);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AEnemyBase::AddToSpawnList, ActivatedTimerDelay, false);
	
	LOG(TEXT("%f로 타이머 초기화합니다"), ActivatedTimerDelay);
}

void AEnemyBase::PlayRandomScareSound()
{
	if (ScareSounds.IsEmpty())
	{
		return;
	}
	
	// 배열에서 랜덤으로 소리를 선택합니다.
	const int32 RandomIndex = FMath::RandRange(0, ScareSounds.Num() - 1);
	USoundCue* SelectedSound = ScareSounds[RandomIndex];
	
	// 선택한 소리를 재생한 후 배열에서 제거합니다.
	USoundLibrary::PlaySFXInLocation(GetOwner(), SelectedSound, EventSpawnLocation);
	ScareSounds.RemoveAt(RandomIndex);
}

void AEnemyBase::GoToStep(int32 StepIndex)
{
	PlayStepAnimation(StepIndex);
	MoveToStepTransform(StepIndex);
}

void AEnemyBase::MoveToNextStep()
{
	++CurrentStepIndex;
	
	if (CurrentStepIndex >= MaxStepIndex)
	{
		LOG(TEXT("플레이어 사망"));
		//StopStepSystem();
	}
	else
	{
		GoToStep(CurrentStepIndex);
	}
}

void AEnemyBase::PlayStepAnimation(uint8 InStepIndex)
{
	// 스텝에 해당하는 애니메이션을 추출합니다.
	UAnimSequence* AnimSequence = EnemyStepInfos[InStepIndex].AnimationSequence;
	
	// 스켈레탈 메시 컴포넌트를 추출합니다.
	USkeletalMeshComponent* MeshComp = GetMesh();
	
	// 스텝에 해당하는 애니메이션을 재생합니다.
	MeshComp->PlayAnimation(AnimSequence, true);
}

void AEnemyBase::MoveToStepTransform(uint8 InStepIndex)
{
	// 스텝에 해당하는 Transform을 추출합니다.
	FTransform TargetTransform = EnemyStepInfos[InStepIndex].TargetTransform;
	
	// 해당 Transform으로 이동시킵니다.
	SetActorTransform(TargetTransform);
}

void AEnemyBase::AddToSpawnList()
{
	if (AMainGameMode* GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (UEventSpawnManager* EventSpawnManager = GameMode->GetEventSpawnManager())
		{
			EventSpawnManager->AddEnemy(this);
		}
	}
}
