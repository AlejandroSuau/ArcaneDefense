#include "Game/ADPlayerController.h"

#include  "Blueprint/UserWidget.h"

AADPlayerController::AADPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	DefaultMouseCursor = EMouseCursor::Default;
}

void AADPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);

	if (!IsLocalController() || !PlayerHUDClass) { return; }

	PlayerHUDWidget = CreateWidget<UUserWidget>(this, PlayerHUDClass);
	if (IsValid(PlayerHUDWidget))
	{
		PlayerHUDWidget->AddToPlayerScreen();
	}
}

