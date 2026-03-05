#include "Actor/Wardrobe.h"

#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AWardrobe::AWardrobe()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	WardrobeDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WardrobeDoor Mesh"));
	WardrobeDoorMesh->SetupAttachment(Root);

	WardrobeDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WardrobeDrawer Mesh"));
	WardrobeDrawerMesh->SetupAttachment(Root);
	
	WardrobeBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WardrobeBody Mesh"));
	WardrobeBodyMesh->SetupAttachment(Root);
	
	WardrobeDoorHandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WardrobeDoorHandle Mesh"));
	WardrobeDoorHandleMesh->SetupAttachment(Root);
	
	PrimaryActorTick.bCanEverTick = true;
}

void AWardrobe::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalLocation = WardrobeDoorHandleMesh->GetComponentLocation();
	TargetLocation = OriginalLocation + FVector(0.0f, 68.0f, 0.0f);
	
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AWardrobe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player) return;

	const FVector Destination = Player->GetIsHiding() ? OriginalLocation : TargetLocation;
	const FVector Current = WardrobeDoorHandleMesh->GetComponentLocation();

	if (!Current.Equals(Destination, 0.1f))
	{
		const FVector NewLocation = FMath::VInterpConstantTo(Current, Destination, DeltaTime, MoveSpeed);
		WardrobeDoorHandleMesh->SetWorldLocation(NewLocation);
	}
}
