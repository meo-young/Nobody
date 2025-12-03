#include "Animation/AnimInstanceBase.h"
#include "Character/CharacterBase.h"
#include "Nobody.h"
#include "Define/Define.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	Player = Cast<ACharacterBase>(TryGetPawnOwner());
	if (!Player)
	{
		LOG(TEXT("PlayerAnimInstance에서 소유한 캐릭터를 찾을 수 없습니다."));
		return;
	}

	MovementComponent = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());
	if (!MovementComponent)
	{
		LOG(TEXT("PlayerAnimInstance에서 캐릭터의 이동 컴포넌트를 찾을 수 없습니다."));
		return;
	}

}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!Player)
	{
		LOG(TEXT("PlayerAnimInstance에서 소유한 캐릭터를 찾을 수 없습니다."));
		Player = Cast<ACharacterBase>(TryGetPawnOwner());
	}

	if (Player && !MovementComponent)
	{
		LOG(TEXT("PlayerAnimInstance에서 캐릭터의 이동 컴포넌트를 찾을 수 없습니다."));
		MovementComponent = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());
	}
}

void UAnimInstanceBase::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	
	if (!Player || !MovementComponent) return;	
	
	// 플레이어의 전방, 우측 벡터를 추출합니다.
	const FVector Forward = Player->GetActorForwardVector();
	const FVector Right   = Player->GetActorRightVector();

	// 월드 기준으로 정규화된 이동 벡터를 추출합니다.
	const FVector WorldDirection = MovementComponent->Velocity.GetSafeNormal();

	// 내적을 계산하여 이동 방향을 구합니다.
	MoveDirection.X = FVector::DotProduct(Forward, WorldDirection);
	MoveDirection.Y = FVector::DotProduct(Right, WorldDirection);

	// 캐릭터의 이동 속도를 계산합니다.
	MoveSpeed = MovementComponent->Velocity.Size();
}