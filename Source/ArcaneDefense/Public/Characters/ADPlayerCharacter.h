#pragma once

#include "CoreMinimal.h"
#include "Characters/ADCharacterBase.h"
#include "InputActionValue.h"
#include "ADPlayerCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UGameplayAbility;
class UADGameplayAbility;
class UADTargetingComponent;
class UADCastComponent;
class UADGroundTargetingComponent;
class UADPlayerResourceAttributeSet;
class UGameplayEffect;
class UADTrapPlacementComponent;
class UADTrapDataAsset;

/**
 * Player-controlled character.
 *
 * Owns player movement, camera setup, and player input bindings.
 */
UCLASS()
class ARCANEDEFENSE_API AADPlayerCharacter : public AADCharacterBase
{
	GENERATED_BODY()

public:
	AADPlayerCharacter();

	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "Resources")
	bool CanAffordCoins(int32 Amount) const;

	UFUNCTION(BlueprintCallable, Category = "Resources")
	bool SpendCoins(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Resources")
	void AddCoins(int32 Amount);
	
	UFUNCTION(BlueprintPure, Category = "Ground Targeting")
	UADGroundTargetingComponent* GetGroundTargetingComponent() const;
	
	UFUNCTION(BlueprintPure, Category = "Targeting")
	UADTargetingComponent* GetTargetingComponent() const;

	UFUNCTION(BlueprintPure, Category = "Resources")
	float GetCoins() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Casting")
	UADCastComponent* GetCastComponent() const;

	UFUNCTION(BlueprintPure, Category = "Abilities|UI")
	bool GetAbilityCooldownInfo(
		TSubclassOf<UGameplayAbility> AbilityClass,
		float& TimeRemaining,
		float& CooldownDuration) const;

	UFUNCTION(BlueprintPure, Category = "Abilities|UI")
	bool CanAffordAbilityCost(TSubclassOf<UGameplayAbility> AbilityClass) const;

	UFUNCTION(BlueprintPure, Category = "Abilities|UI")
	UADGameplayAbility* GetAbilityInstance(TSubclassOf<UGameplayAbility> AbilityClass) const;
	
protected:
	virtual void BeginPlay() override;
	
	/** Camera boom that keeps the camera behind the character. */
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Camera",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Third-person follow camera. */
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Camera",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Mapping context used during normal player gameplay. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<UInputMappingContext> PlayerMappingContext;

	/** Two-dimensional movement input. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<UInputAction> MoveAction;

	/** Two-dimensional camera input. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<UInputAction> LookAction;

	/** Jump input. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true")
	)
	TObjectPtr<UInputAction> JumpAction;

	/** Selects an enemy under the mouse cursor */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SelectTargetAction;
	
	/** Enables camera rotation while held. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CameraLookAction;

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UADGroundTargetingComponent> GroundTargetingComponent;
	
	/** Handles enemy target selection. */
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Combat",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UADTargetingComponent> TargetingComponent;

	/** Exposes active cast information to UI and presentation systems. */
	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Combat",
		meta = (AllowPrivvateAccess = "true"))
	TObjectPtr<UADCastComponent> CastComponent;
		
	/** Activates the first gameplay ability slot. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Ability1Action;

	/** Activates the second gameplay ability slot. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Ability2Action;

	/** Activates the third gameplay ability slot. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Ability3Action;

	/** Activates the fourth gameplay ability slot. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Ability4Action;

	/** Ability granted and activated through the first ability slot. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Abilities",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> Ability1Class;

	/** Ability granted and activated through the second ability slot. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Abilities",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> Ability2Class;

	/** Ability granted and activated through the third ability slot. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Abilities",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> Ability3Class;

	/** Ability granted and activated through the fourth ability slot. */
	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Abilities",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> Ability4Class;

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Ability System",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UADPlayerResourceAttributeSet> PlayerResourceAttributes;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability System|Initialization",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> InitialResourcesEffect;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Ability System|Resources",
		meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> CoinModificationEffect;

	UPROPERTY(
		VisibleAnywhere,
		BlueprintReadOnly,
		Category = "Components",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UADTrapPlacementComponent> TrapPlacementComponent;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Input",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Trap1Action;

	UPROPERTY(
		EditDefaultsOnly,
		BlueprintReadOnly,
		Category = "Traps",
		meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UADTrapDataAsset> Trap1Data;
	
	bool bCameraLookActive = false;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void SelectTarget(const FInputActionValue& Value);
	void StartCameraLook(const FInputActionValue& Value);
	void StopCameraLook(const FInputActionValue& Value);

	void ActivateAbility1(const FInputActionValue& Value);
	void ActivateAbility2(const FInputActionValue& Value);
	void ActivateAbility3(const FInputActionValue& Value);
	void ActivateAbility4(const FInputActionValue& Value);

	void ActivateTrap1();
	
	void ApplyInitialResources();
	bool ApplyCoinDelta(float Delta);
	
	void CancelAbilitiesInterruptedByMovement();

	void GrantStartupAbility(TSubclassOf<UGameplayAbility> AbilityClass);
};