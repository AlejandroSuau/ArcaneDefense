#include "Characters/ADPlayerCharacter.h"

#include "Camera/CameraComponent.h"

#include "Combat/ADTargetingComponent.h"
#include "Combat/ADCastComponent.h"
#include "Combat/ADGroundTargetingComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "Engine/LocalPlayer.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "AbilitySystem/Abilities/ADGameplayAbility.h"
#include "AbilitySystem/Abilities/ADGA_GroundTargetedArea.h"
#include "AbilitySystem/ADGameplayTags.h"
#include "AbilitySystem/ADPlayerResourceAttributeSet.h"

AADPlayerCharacter::AADPlayerCharacter()
{
	// The controller rotates the camera, not the character directly.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	MovementComponent->MaxWalkSpeed = 500.0f;
	MovementComponent->JumpZVelocity = 700.0f;
	MovementComponent->AirControl = 0.35f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GroundTargetingComponent = CreateDefaultSubobject<UADGroundTargetingComponent>(TEXT("GroundTargetingComponent"));
	TargetingComponent = CreateDefaultSubobject<UADTargetingComponent>(TEXT("TargetingComponent"));
	CastComponent = CreateDefaultSubobject<UADCastComponent>(TEXT("CastComponent"));
	PlayerResourceAttributes = CreateDefaultSubobject<UADPlayerResourceAttributeSet>(TEXT("PlayerResourceAttributes"));
}

void AADPlayerCharacter::ApplyInitialResources()
{
	if (!HasAuthority() || !InitialResourcesEffect)	{ return; }

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC)) { return; }

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(
		InitialResourcesEffect,
		1.0f,
		EffectContext);

	if (!EffectSpec.IsValid()) { return; }
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}

void AADPlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (!IsValid(PlayerController)) { return; }

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!IsValid(LocalPlayer)) { return; }

	auto* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!IsValid(InputSubsystem) || !IsValid(PlayerMappingContext)) { return; }

	// Avoid keeping a stale copy when the pawn is restarted.
	InputSubsystem->RemoveMappingContext(PlayerMappingContext);
	InputSubsystem->AddMappingContext(PlayerMappingContext, 0);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("IMC ADDED: %s"),
		*GetNameSafe(PlayerMappingContext)
	);
}

void AADPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UE_LOG(LogTemp, Warning, TEXT("INPUT SETUP OK"));

	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!ensureMsgf(
		IsValid(EnhancedInputComponent),
		TEXT("AADPlayerCharacter requires an Enhanced Input Component.")))
	{
		return;
	}

	if (IsValid(MoveAction))
	{
		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AADPlayerCharacter::Move
		);
	}

	if (IsValid(LookAction))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("LOOK ACTION BOUND: %s"),
			*GetNameSafe(LookAction)
		);
		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&AADPlayerCharacter::Look
		);
	}

	if (IsValid(JumpAction))
	{
		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Started,
			this,
			&AADPlayerCharacter::StartJump
		);

		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			this,
			&AADPlayerCharacter::StopJump
		);
	}

	if (IsValid(SelectTargetAction))
	{
		EnhancedInputComponent->BindAction(
			SelectTargetAction,
			ETriggerEvent::Started,
			this,
			&AADPlayerCharacter::SelectTarget);
	}

	if (IsValid(CameraLookAction))
	{
		EnhancedInputComponent->BindAction(
			CameraLookAction,
			ETriggerEvent::Started,
			this,
			&AADPlayerCharacter::StartCameraLook);

		EnhancedInputComponent->BindAction(
			CameraLookAction,
			ETriggerEvent::Completed,
			this,
			&AADPlayerCharacter::StopCameraLook);

		EnhancedInputComponent->BindAction(
			CameraLookAction,
			ETriggerEvent::Canceled,
			this,
			&AADPlayerCharacter::StopCameraLook);
	}

	if (IsValid(Ability1Action))
	{
		EnhancedInputComponent->BindAction(
			Ability1Action,
			ETriggerEvent::Started,
			this,
			&AADPlayerCharacter::ActivateAbility1);
	}

	if (IsValid(Ability2Action))
	{
		EnhancedInputComponent->BindAction(
			Ability2Action,
			ETriggerEvent::Started,
			this,
			&AADPlayerCharacter::ActivateAbility2);
	}

	if (IsValid(Ability3Action))
	{
		EnhancedInputComponent->BindAction(
			Ability3Action,
			ETriggerEvent::Started,
			this,
			&AADPlayerCharacter::ActivateAbility3
		);
	}

	if (IsValid(Ability4Action))
	{
		EnhancedInputComponent->BindAction(
			Ability4Action,
			ETriggerEvent::Started,
			this,
			&AADPlayerCharacter::ActivateAbility4
		);
	}
}

void AADPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!IsValid(Controller)) { return; }

	const FVector2D MovementInput = Value.Get<FVector2D>();
	if (!MovementInput.IsNearlyZero())
	{
		CancelAbilitiesInterruptedByMovement();
	}

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, ControlRotation.Yaw, 0.0f);
	const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementInput.Y);
	AddMovementInput(RightDirection, MovementInput.X);
}

void AADPlayerCharacter::Look(const FInputActionValue& Value)
{
	if (!bCameraLookActive) { return; }

	const FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void AADPlayerCharacter::StartJump(const FInputActionValue& /*Value*/)
{
	CancelAbilitiesInterruptedByMovement();

	Jump();
}

void AADPlayerCharacter::StopJump(const FInputActionValue& /*Value*/)
{
	StopJumping();
}

UADGroundTargetingComponent* AADPlayerCharacter::GetGroundTargetingComponent() const
{
	return GroundTargetingComponent;
}

UADTargetingComponent* AADPlayerCharacter::GetTargetingComponent() const
{
	return TargetingComponent;
}

UADCastComponent* AADPlayerCharacter::GetCastComponent() const
{
	return CastComponent;
}

void AADPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ApplyInitialResources();

	GrantStartupAbility(Ability1Class);
	GrantStartupAbility(Ability2Class);
	GrantStartupAbility(Ability3Class);
	GrantStartupAbility(Ability4Class);
}

void AADPlayerCharacter::GrantStartupAbility(TSubclassOf<UGameplayAbility> AbilityClass)
{
	if (!HasAuthority() || !AbilityClass) { return; }

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC)) { return; }

	if (ASC->FindAbilitySpecFromClass(AbilityClass)) { return; }

	FGameplayAbilitySpec AbilitySpec(
		AbilityClass,
		1,
		INDEX_NONE,
		this
	);

	ASC->GiveAbility(AbilitySpec);
}

void AADPlayerCharacter::SelectTarget(const FInputActionValue& /*Value*/)
{
	if (IsValid(GroundTargetingComponent) && GroundTargetingComponent->IsTargeting())
	{
		if (GroundTargetingComponent->ConfirmTargeting())
		{
			auto* ASC = GetAbilitySystemComponent();

			const bool bActivated = (
				IsValid(ASC)
				&& Ability4Class
				&& ASC->TryActivateAbilityByClass(Ability4Class));

			if (!bActivated)
			{
				GroundTargetingComponent->ClearConfirmedLocation();
			}
		}

		return;
	}
	
	if (IsValid(TargetingComponent))
	{
		TargetingComponent->TrySelectTargetUnderCursor();
	}
}

void AADPlayerCharacter::StartCameraLook(const FInputActionValue& Value)
{
	if (IsValid(GroundTargetingComponent) && GroundTargetingComponent->IsTargeting())
	{
		GroundTargetingComponent->CancelTargeting();
		return;
	}
	
	bCameraLookActive = true;

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!IsValid(PlayerController)) { return; }

	PlayerController->bShowMouseCursor = false;
	PlayerController->SetInputMode(FInputModeGameOnly());
}

void AADPlayerCharacter::StopCameraLook(const FInputActionValue& Value)
{
	bCameraLookActive = false;

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!IsValid(PlayerController)) { return; }

	PlayerController->bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	PlayerController->SetInputMode(InputMode);
}

void AADPlayerCharacter::ActivateAbility1(const FInputActionValue& /*Value*/)
{
	if (IsValid(GroundTargetingComponent) && GroundTargetingComponent->IsTargeting())
	{
		return;
	}
	
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponent();
	if (!IsValid(AbilitySystem) || !Ability1Class) { return; }

	const bool bActivationStarted = AbilitySystem->TryActivateAbilityByClass(
		Ability1Class, true);
	if (!bActivationStarted)
	{
		UE_LOG(
			LogTemp,
			Verbose,
			TEXT("%s  could not activate ability %s."),
			*GetNameSafe(this),
			*GetNameSafe(Ability1Class));
	}
}

void AADPlayerCharacter::ActivateAbility2(const FInputActionValue& /*Value*/)
{
	if (IsValid(GroundTargetingComponent) && GroundTargetingComponent->IsTargeting())
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC) || !Ability2Class) { return; }

	ASC->TryActivateAbilityByClass(Ability2Class);
}

void AADPlayerCharacter::ActivateAbility3(const FInputActionValue& /*Value*/)
{
	if (IsValid(GroundTargetingComponent) && GroundTargetingComponent->IsTargeting())
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC) || !Ability3Class) { return; }

	ASC->TryActivateAbilityByClass(Ability3Class);
}

void AADPlayerCharacter::ActivateAbility4(const FInputActionValue& /*Value*/)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (IsValid(ASC) && ASC->HasMatchingGameplayTag(ADGameplayTags::State_Casting))
	{
		return;
	}
	
	if (!IsValid(GroundTargetingComponent) || !Ability4Class) { return; }

	if (GroundTargetingComponent->IsTargeting())
	{
		GroundTargetingComponent->CancelTargeting();
		return;
	}

	const auto* GroundAbility = Cast<UADGA_GroundTargetedArea>(
		Ability4Class->GetDefaultObject());
	if (!IsValid(GroundAbility) || !GroundAbility->GetTargetPreviewActorClass())
	{
		return;
	}

	GroundTargetingComponent->StartTargeting(
		GroundAbility->GetTargetingRange(),
		GroundAbility->GetEffectRadius(),
		GroundAbility->GetTargetPreviewActorClass());
}

void AADPlayerCharacter::CancelAbilitiesInterruptedByMovement()
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponent();
	if (IsValid(AbilitySystem) &&
		AbilitySystem->HasMatchingGameplayTag(ADGameplayTags::State_Casting))
	{
		FGameplayTagContainer AbilitiesToCancel;
		AbilitiesToCancel.AddTag(ADGameplayTags::Ability_CancelOnMovement);
		AbilitySystem->CancelAbilities(&AbilitiesToCancel, nullptr, nullptr);
	}
}

UADGameplayAbility* AADPlayerCharacter::GetAbilityInstance(
	TSubclassOf<UGameplayAbility> AbilityClass) const
{
	if (!AbilityClass) { return nullptr; }

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC)) { return nullptr; }

	auto* Spec = ASC->FindAbilitySpecFromClass(AbilityClass);
	if (!Spec) { return nullptr; }

	return Cast<UADGameplayAbility>(Spec->GetPrimaryInstance());
}

bool AADPlayerCharacter::GetAbilityCooldownInfo(
	TSubclassOf<UGameplayAbility> AbilityClass,
	float& TimeRemaining,
	float& CooldownDuration) const
{
	TimeRemaining = 0.0f;
	CooldownDuration = 0.0f;

	UADGameplayAbility* Ability = GetAbilityInstance(AbilityClass);
	if (!IsValid(Ability)) { return false; }

	const auto* Spec = GetAbilitySystemComponent()->FindAbilitySpecFromClass(AbilityClass);
	if (!Spec) { return false; }

	const auto* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo) { return false;	}

	Ability->GetCooldownTimeRemainingAndDuration(
		Spec->Handle,
		ActorInfo,
		TimeRemaining,
		CooldownDuration);

	return (TimeRemaining > 0.0f && CooldownDuration > 0.0f);
}

bool AADPlayerCharacter::CanAffordAbilityCost(
	TSubclassOf<UGameplayAbility> AbilityClass) const
{
	UADGameplayAbility* Ability = GetAbilityInstance(AbilityClass);
	if (!IsValid(Ability)) { return false; }

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC)) { return false; }

	auto* Spec = ASC->FindAbilitySpecFromClass(AbilityClass);
	if (!Spec) { return false; }

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo)	{ return false; }

	return Ability->CheckCost(Spec->Handle, ActorInfo, nullptr);
}

float AADPlayerCharacter::GetCoins() const
{
	return IsValid(PlayerResourceAttributes)
		? PlayerResourceAttributes->GetCoins()
		: 0.0f;
}