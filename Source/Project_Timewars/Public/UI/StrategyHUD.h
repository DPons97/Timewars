// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StrategyHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AStrategyHUD : public AHUD
{
	GENERATED_BODY()

public:
	void StartSelection();
	
	void EndSelection();

	bool DrawSelectionBox();
	void DrawHUD() override;

	TArray<class ASelectableActor*> GetCurrentSelection();

	UPROPERTY(EditDefaultsOnly)
	float SelectionBorderThickness = 1.f;

protected:

	
private:
	bool GetMousePosition(FVector2D& MousePosition);
	
	bool isSelecting = false;

	// Storing starting and ending selection points
	FVector2D SelectionStartPoint;

	FVector2D SelectionEndPoint;

	TArray<ASelectableActor*> LastSelection;
};