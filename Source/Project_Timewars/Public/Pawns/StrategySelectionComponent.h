// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategyHUD.h"
#include "Components/ActorComponent.h"
#include "StrategySelectionComponent.generated.h"

class IStrategyCommandInterface;
class ASelectablePawn;


/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API UStrategySelectionComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	UStrategySelectionComponent();

	/**
	 *	Get the strategy command interfaces of the passed selection.
	 *	Keep in mind: the IStrategyCommandInterface is implemented by controllers,
	 *						therefore this method shall be only called inside the server
	 *	@param Selection from which to get the interfaces 						
	 *	@return TArray of IStrategyCommandInterface references to control currently selected actors.
	 *			If running without Authority, returns empty array
	 */
	TArray<IStrategyCommandInterface*> GetCurrentSelectionControllers(TArray<ASelectablePawn*> Selection);

	TArray<ASelectablePawn*> GetCurrentSelection() const { return SelectedActors; }
	
    void EndSelection();

	void StartSelection();

protected:
	void BeginPlay() override;
	
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction);

	/*
     * Maximum approximation to select an actor
     */
    UPROPERTY(EditDefaultsOnly)
    float MaxSelectionApprox = 30.f;
    
private:
	TArray<ASelectablePawn*> MakeSingleSelection();
	
	TArray<ASelectablePawn*> MakeMultipleSelection();

	void ResetSelection();

	void SelectActors(TArray<ASelectablePawn*> selectedActors);

	APlayerController* PlayerController;

	AStrategyHUD* HUD;

	/* Array of last selection (pawns) */
	TArray<ASelectablePawn*> SelectedActors;

	bool isSelecting;

	// Storing starting and ending selection points
	FVector2D SelectionStartPoint;

	FVector2D SelectionEndPoint;
};
