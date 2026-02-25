#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "FamilyDoll.generated.h"

class UBoxComponent;
class AEnemyBase;

UCLASS()
class NOBODY_API AFamilyDoll : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:
	AFamilyDoll();
	
public:
	/** 상호작용을 수행하는 함수입니다. */
	virtual void Interact_Implementation() override;
	
	virtual EInteractionType GetInteractionType() override;
	
protected:
	/** 인형에 맞는 소리를 재생하는 함수입니다. */
	void PlayDollSound();
	
	/** 이벤트가 활성화되어 있을 경우 소리를 재생하는 함수입니다. */
	void PlayNoiseSound();
	
	/** 상호작용 콜리전을 활성화하는 함수입니다. */
	void ActivateCollision();
	
protected:
	/** 인형이 낼 수 있는 소리 배열 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	TArray<TObjectPtr<USoundCue>> DollSounds;
	
	/** 인형이 내는 노이즈 소리 변수입니다. */
	UPROPERTY(EditDefaultsOnly, Category = "변수")
	TObjectPtr<USoundCue> NoiseSound;
	
	/** 이 인형의 이벤트 주체에 대한 변수입니다. */
	UPROPERTY(EditInstanceOnly, Category = "변수")
	TObjectPtr<AEnemyBase> TargetEnemy;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> DollMesh;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> InteractionZone;
	
	EInteractionType InteractionType;
	
private:
	FTimerHandle CollisionTimerHandle;

};
