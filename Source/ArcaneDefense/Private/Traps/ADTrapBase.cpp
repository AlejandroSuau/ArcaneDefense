#include "Traps/ADTrapBase.h"

#include "AbilitySystemComponent.h"
#include "Characters/ADEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Traps/ADTrapDataAsset.h"
#include "Traps/ADTrapPlacementSlot.h"

AADTrapBase::AADTrapBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	ActivationVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ActivationVolume"));
	ActivationVolume->SetupAttachment(SceneRoot);
	ActivationVolume->InitBoxExtent(FVector(100.0f,100.0f,100.0f));
	ActivationVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ActivationVolume->SetCollisionObjectType(ECC_WorldDynamic);
	ActivationVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	ActivationVolume->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	ActivationVolume->SetGenerateOverlapEvents(true);
}

void AADTrapBase::BeginPlay()
{
	Super::BeginPlay();

	ActivationVolume->OnComponentBeginOverlap.AddUniqueDynamic(
		this, &AADTrapBase::HandleActivationBeginOverlap);

	ActivationVolume->OnComponentEndOverlap.AddUniqueDynamic(
		this, &AADTrapBase::HandleActivationEndOverlap);

	if (IsValid(TrapData))
	{
		ApplyTrapData();
	}
}

void AADTrapBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(ActivationVolume))
	{
		ActivationVolume->OnComponentBeginOverlap.RemoveDynamic(
			this, &AADTrapBase::HandleActivationBeginOverlap);

		ActivationVolume->OnComponentEndOverlap.RemoveDynamic(
			this, &AADTrapBase::HandleActivationEndOverlap);
	}

	if (AADTrapPlacementSlot* Slot = PlacementSlot.Get())
	{
		Slot->Release(this);
	}

	PlacementSlot.Reset();
	
	Super::EndPlay(EndPlayReason);
}

void AADTrapBase::InitializeTrap(
	UADTrapDataAsset* InTrapData,
	UAbilitySystemComponent* InSourceAbilitySystem,
	const int32 InPurchasePrice,
	AADTrapPlacementSlot* InPlacementSlot)
{
	if (!IsValid(InTrapData) || !IsValid(InPlacementSlot)) { return; }

	TrapData = InTrapData;
	SourceAbilitySystem = InSourceAbilitySystem;
	PurchasePrice = FMath::Max(0,InPurchasePrice);
	PlacementSlot = InPlacementSlot;

	ApplyTrapData();
}

void AADTrapBase::ApplyTrapData()
{
	if (!IsValid(TrapData) || !IsValid(ActivationVolume)) { return; }

	AADTrapPlacementSlot* Slot = PlacementSlot.Get();
	if (!IsValid(Slot)) { return; }

	const FVector SlotExtent = Slot->GetPlacementBlockingHalfExtent();
	const float HalfRange = TrapData->ActivationRange * 0.5f;

	ActivationVolume->SetBoxExtent(
		FVector(SlotExtent.X, SlotExtent.Y,HalfRange),true);
	ActivationVolume->SetRelativeLocation(FVector(0.0f,0.0f,HalfRange));
}

void AADTrapBase::HandleActivationBeginOverlap(
	UPrimitiveComponent* /*OverlappedComponent*/,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 /*OtherBodyIndex*/,
	bool /*bFromSweep*/,
	const FHitResult& /*SweepResult*/)
{
	AADEnemyCharacter* Enemy = Cast<AADEnemyCharacter>(OtherActor);
	if (!IsValid(Enemy) || Enemy->IsDead())	{ return; }

	/*
	 * Only use the Character capsule as the gameplay overlap source.
	 * This avoids duplicate events from additional enemy components.
	 */
	if (OtherComponent != Enemy->GetCapsuleComponent())	{ return; }

	HandleEnemyEnteredTrigger(Enemy);
}

void AADTrapBase::HandleActivationEndOverlap(
	UPrimitiveComponent* /*OverlappedComponent*/,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	int32 /*OtherBodyIndex*/)
{
	AADEnemyCharacter* Enemy = Cast<AADEnemyCharacter>(OtherActor);
	if (!IsValid(Enemy) || OtherComponent != Enemy->GetCapsuleComponent()) { return; }

	HandleEnemyExitedTrigger(Enemy);
}

void AADTrapBase::HandleEnemyEnteredTrigger(AADEnemyCharacter* Enemy)
{
	ReceiveEnemyEnteredTrigger(Enemy);
}

void AADTrapBase::HandleEnemyExitedTrigger(AADEnemyCharacter* Enemy)
{
	ReceiveEnemyExitedTrigger(Enemy);
}

void AADTrapBase::GetValidEnemiesInActivationRange(TArray<AADEnemyCharacter*>& OutEnemies) const
{
	OutEnemies.Reset();

	if (!IsValid(ActivationVolume))	{ return; }

	TArray<AActor*> OverlappingActors;
	ActivationVolume->GetOverlappingActors(
		OverlappingActors, AADEnemyCharacter::StaticClass());

	for (auto* OverlappingActor : OverlappingActors)
	{
		auto* Enemy = Cast<AADEnemyCharacter>(OverlappingActor);
		if (!IsValid(Enemy) || Enemy->IsDead()) { continue; }

		OutEnemies.Add(Enemy);
	}
}

UADTrapDataAsset* AADTrapBase::GetTrapData() const
{
	return TrapData;
}

int32 AADTrapBase::GetPurchasePrice() const
{
	return PurchasePrice;
}

float AADTrapBase::GetSellRefundRatio() const
{
	return IsValid(TrapData) ? TrapData->SellRefundRatio : 0.0f;
}

int32 AADTrapBase::GetExpectedSellValue() const
{
	if (!IsValid(TrapData)) { return 0; }

	return FMath::RoundToInt(static_cast<float>(PurchasePrice) * TrapData->SellRefundRatio);
}

float AADTrapBase::GetActivationRange() const
{
	return IsValid(TrapData) ? TrapData->ActivationRange : 0.0f;
}

int32 AADTrapBase::GetValidEnemyCount() const
{
	TArray<AADEnemyCharacter*> Enemies;
	GetValidEnemiesInActivationRange(Enemies);

	return Enemies.Num();
}

UAbilitySystemComponent* AADTrapBase::GetSourceAbilitySystem() const
{
	return SourceAbilitySystem.Get();
}