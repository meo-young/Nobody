#include "Subsystem/SoundSubsystem.h"
#include "Nobody.h"
#include "TableRowBase/BGMTableRowBase.h"
#include "TableRowBase/SFXTableRowBase.h"
#include "Enum/EBGM.h"
#include "Enum/ESFX.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

void USoundSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	SFXDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/_Nobody/DataTable/DT_SFX"));
	BGMDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/_Nobody/DataTable/DT_BGM"));
}

void USoundSubsystem::Deinitialize()
{
	SFXQueues.Empty();
	GCPreventionArray.Empty();

	Super::Deinitialize();
}

void USoundSubsystem::Init()
{
	// 오디오 컴포넌트를 등록합니다.
	RegisterSFXComponent();
	RegisterBGMComponent();

	// 사운드 데이터테이블의 모든 행을 가져옵니다.
	SFXDataTable->GetAllRows("SFX", SFXTableRows);
	BGMDataTable->GetAllRows("BGM", BGMTableRows);
}

void USoundSubsystem::PlaySFXInLocation(ESFX SFXType, const FVector& Location)
{
	// 유효한 SFXType인지 확인합니다.
	if (!SFXTableRows.IsValidIndex(static_cast<int32>(SFXType)))
	{
		LOG(TEXT("USoundSubsystem: Invalid SFX Type %d"), static_cast<int32>(SFXType));
		return;
	}

	// 동기 로드를 사용하여 사운드를 로드합니다.
	USoundCue* LoadedSound = SFXTableRows[static_cast<int32>(SFXType)]->Sound.LoadSynchronous();
	if (!IsValid(LoadedSound))
	{
		LOG(TEXT("USoundSubsystem: Failed to load SFX %d"), static_cast<int32>(SFXType));
		return;
	}

	// 풀에서 오디오 컴포넌트를 가져옵니다.
	UAudioComponent* AC = GetPooledAudioComponent();
	if (!IsValid(AC))
	{
		LOG(TEXT("USoundSubsystem: Failed to get audio component"));
		return;
	}

	// 사운드를 설정하고 재생합니다.
	AC->SetSound(LoadedSound);
	AC->SetWorldLocation(Location);
	AC->Play();
}

void USoundSubsystem::PlaySFXInLocation(USoundCue* InSound, const FVector& Location)
{
	// 풀에서 오디오 컴포넌트를 가져옵니다.
	UAudioComponent* AC = GetPooledAudioComponent();
	if (!IsValid(AC))
	{
		LOG(TEXT("USoundSubsystem: Failed to get audio component"));
		return;
	}

	// 사운드를 설정하고 재생합니다.
	AC->SetSound(InSound);
	AC->SetWorldLocation(Location);
	AC->Play();
}

void USoundSubsystem::PlaySFX2D(ESFX SFXType)
{
	PlaySFXInLocation(SFXType, FVector::ZeroVector);
}

void USoundSubsystem::PlaySFX2D(USoundCue* InSound)
{
	PlaySFXInLocation(InSound, FVector::ZeroVector);
}

void USoundSubsystem::PlayBGM(EBGM BGMType)
{
	// 유효한 BGMType인지 확인합니다.
	if (!BGMAudioComponent)
	{
		LOG(TEXT("USoundSubsystem: BGM AudioComponent is invalid"));
		return;
	}

	if (!BGMTableRows.IsValidIndex(static_cast<int32>(BGMType)))
	{
		LOG(TEXT("USoundSubsystem: Invalid BGM Type %d"), static_cast<int32>(BGMType));
		return;
	}
	
	// 동기 로드를 사용하여 사운드를 로드합니다.
	USoundCue* LoadedSound = BGMTableRows[static_cast<int32>(BGMType)]->Sound.LoadSynchronous();
	if (!IsValid(LoadedSound))
	{
		LOG(TEXT("USoundSubsystem: Failed to load BGM %d"), static_cast<int32>(BGMType));
		return;
	}

	// 기존 BGM이 재생 중이면 정지합니다.
	if (BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->Stop();
	}

	// 사운드를 설정하고 재생합니다.
	BGMAudioComponent->SetSound(LoadedSound);
	BGMAudioComponent->Play();
}

void USoundSubsystem::StopBGM()
{
	if (BGMAudioComponent && BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->Stop();
	}
}

void USoundSubsystem::PauseBGM()
{
	if (BGMAudioComponent && BGMAudioComponent->IsPlaying())
	{
		BGMAudioComponent->SetPaused(true);
	}
}

void USoundSubsystem::ResumeBGM()
{
	if (BGMAudioComponent && BGMAudioComponent->bIsPaused)
	{
		BGMAudioComponent->SetPaused(false);
	}
}

void USoundSubsystem::OnSFXFinished(UAudioComponent* AudioComponent)
{
	if (AudioComponent)
	{
		AudioComponent->Stop();
		SFXQueues.Enqueue(AudioComponent);
	}
}

UAudioComponent* USoundSubsystem::GetPooledAudioComponent()
{
	UAudioComponent* AC = nullptr;

	// 큐에서 사용 가능한 오디오 컴포넌트를 가져옵니다.
	// 없다면 새로 생성합니다.
	if (!SFXQueues.Dequeue(AC))
	{
		UWorld* CachedWorld = GetWorld();
		AC = NewObject<UAudioComponent>(CachedWorld);
		AC->bAutoActivate = false;
		AC->OnAudioFinishedNative.AddLambda([this](UAudioComponent* Comp) { OnSFXFinished(Comp); });
		AC->RegisterComponentWithWorld(CachedWorld);
		GCPreventionArray.Add(AC);
	}

	return AC;
}

void USoundSubsystem::RegisterSFXComponent()
{
	// 오디오 컴포넌트 풀을 초기화합니다.
	SFXQueues.Empty();

	// 월드를 캐싱합니다.
	UWorld* CachedWorld = GetWorld();

	// 미리 생성된 오디오 컴포넌트를 큐에 추가합니다.
	for (int8 i = 0; i < 30; ++i)
	{
		UAudioComponent* AC = NewObject<UAudioComponent>(CachedWorld);
		AC->bAutoActivate = false;
		AC->OnAudioFinishedNative.AddLambda([this](UAudioComponent* Comp) { OnSFXFinished(Comp); });
		AC->RegisterComponentWithWorld(CachedWorld);
		SFXQueues.Enqueue(AC);
		GCPreventionArray.Add(AC);
	}
}

void USoundSubsystem::RegisterBGMComponent()
{
	// 월드를 캐싱합니다.
	UWorld* CachedWorld = GetWorld();

	// BGM 오디오 컴포넌트를 생성합니다.
	BGMAudioComponent = NewObject<UAudioComponent>(CachedWorld);
	BGMAudioComponent->bAutoActivate = false;
	BGMAudioComponent->RegisterComponentWithWorld(CachedWorld);
	
	GCPreventionArray.Add(BGMAudioComponent);
}