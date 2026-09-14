#pragma once

#include "CoreMinimal.h"
#include "Characters/ADCharacterBase.h"
#include "InputActionValue.h"
#include "ADPlayerCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UADTargetingComponent;
class UGameplayAbility;
class UADCastComponent;

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

	UFUNCTION(BlueprintPure, Category = "Targeting")
	UADTargetingComponent* GetTargetingComponent() const;

	UFUNCTION(BlueprintPure, Category = "Combat|Casting")
	UADCastComponent* GetCastComponent() const;
	
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
	void CancelAbilitiesInterruptedByMovement();

	void GrantStartupAbility(TSubclassOf<UGameplayAbility> AbilityClass);
};