#include "Traps/ADTrapBase.h"

#include "AbilitySystemComponent.h"
#include "Characters/ADEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Traps/ADTrapDataAsset.h"

AADTrapBase::AADTrapBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(SceneRoot);
	TriggerVolume->InitBoxExtent(FVector(100.0f, 100.0f, 75.0f));
	TriggerVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerVolume->SetCollisionObjectType(ECC_WorldDynamic);
	TriggerVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerVolume->SetGenerateOverlapEvents(true);
}

void AADTrapBase::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddUniqueDynamic(
		this, &AADTrapBase::HandleTriggerBeginOverlap);

	TriggerVolume->OnComponentEndOverlap.AddUniqueDynamic(
		this, &AADTrapBase::HandleTriggerEndOverlap);

	if (IsValid(TrapData))
	{
		ApplyTrapData();
	}
}

void AADTrapBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(TriggerVolume))
	{
		TriggerVolume->OnComponentBeginOverlap.RemoveDynamic(
			this, &AADTrapBase::HandleTriggerBeginOverlap);

		TriggerVolume->OnComponentEndOverlap.RemoveDynamic(
			this, &AADTrapBase::HandleTriggerEndOverlap);
	}

	Super::EndPlay(EndPlayReason);
}

void AADTrapBase::InitializeTrap(
	UADTrapDataAsset* InTrapData,
	UAbilitySystemComponent* InSourceAbilitySystem,
	const int32 InPurchasePrice)
{
	if (!IsValid(InTrapData)) { return;	}

	TrapData = InTrapData;
	SourceAbilitySystem = InSourceAbilitySystem;
	PurchasePrice = FMath::Max(0, InPurchasePrice);

	ApplyTrapData();
}

void AADTrapBase::ApplyTrapData()
{
	if (!IsValid(TrapData) || !IsValid(TriggerVolume)) { return; }

	TriggerVolume->SetBoxExtent(TrapData->TriggerBoxExtent, true);
}

void AADTrapBase::HandleTriggerBeginOverlap(
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

void AADTrapBase::HandleTriggerEndOverlap(
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

void AADTrapBase::GetValidEnemiesInTrigger(TArray<AADEnemyCharacter*>& OutEnemies) const
{
	OutEnemies.Reset();

	if (!IsValid(TriggerVolume))	{ return; }

	TArray<AActor*> OverlappingActors;
	TriggerVolume->GetOverlappingActors(
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

int32 AADTrapBase::GetValidEnemyCount() const
{
	TArray<AADEnemyCharacter*> Enemies;
	GetValidEnemiesInTrigger(Enemies);

	return Enemies.Num();
}

UAbilitySystemComponent* AADTrapBase::GetSourceAbilitySystem() const
{
	return SourceAbilitySystem.Get();
}