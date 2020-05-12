// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/StrategySelectionComponent.h"

#include "DrawDebugHelpers.h"
#include "SelectableActor.h"
#include "StrategyHUD.h"
#include "TimewarsSpectatorPawn.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"

UStrategySelectionComponent::UStrategySelectionComponent()
{
    isSelecting = false;
}

void UStrategySelectionComponent::BeginPlay()
{
    PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!ensure(PlayerController != nullptr)) return;

    HUD = Cast<AStrategyHUD>(PlayerController->GetHUD());
}

void UStrategySelectionComponent::TickComponent(float DeltaTime,enum ELevelTick TickType,FActorComponentTickFunction * ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UStrategySelectionComponent::StartSelection()
{
    if (!ensure(HUD != nullptr)) return;
    if (!GetMousePosition(SelectionStartPoint)) return;

    // Block camera from moving
    UStrategyMovementComponent* MovementComponent = Cast<UStrategyMovementComponent>(Cast<ATimewarsSpectatorPawn>(GetOwner())->GetMovementComponent());
    if (!MovementComponent) return;
    MovementComponent->bCanMoveCamera = false;
    
    HUD->StartSelection();    
    isSelecting = true;
}

void UStrategySelectionComponent::EndSelection()
{
    auto World = GetWorld();
    if (!ensure(World != nullptr)) return;
    if (!ensure(HUD != nullptr)) return;

    // Get selection endpoint
    if (!GetMousePosition(SelectionEndPoint))
    {
        return;
    }

    TArray<ASelectableActor*> Selection;
    
    // If end and start points are near enough, select only as 1 point
    if (FVector2D::Distance(SelectionEndPoint, SelectionStartPoint) <= MaxSelectionApprox)
    {
        Selection = MakeSingleSelection();
    } else
    {
        // Else select from area
        Selection = MakeMultipleSelection();
    }

    HUD->EndSelection(); 
    ResetSelection();
    SelectActors(Selection);

    // Re-allow camera movement
    UStrategyMovementComponent* MovementComponent = Cast<UStrategyMovementComponent>(Cast<ATimewarsSpectatorPawn>(GetOwner())->GetMovementComponent());
    if (!MovementComponent) return;
    MovementComponent->bCanMoveCamera = true;

    //UE_LOG(LogTemp, Warning, TEXT("Selected actors: %d"), Selection.Num())
}

bool UStrategySelectionComponent::GetMousePosition(FVector2D &outMousePosition)
{
    if (!ensure(PlayerController != nullptr)) return false;

    float mouseX, mouseY;
    if (!PlayerController->GetMousePosition(mouseX, mouseY))
    {
        return false;
    }

    outMousePosition = FVector2D(mouseX, mouseY);
    return true;
}

bool UStrategySelectionComponent::DeprojectPositionToWorld(FVector2D position, FVector &outWorldPosition)
{
    if (!ensure(PlayerController != nullptr)) return false;

    // Ray trace mouse to world
    FHitResult HitResult;
    FCollisionQueryParams Params;
    if (!PlayerController->GetHitResultAtScreenPosition(position, ECollisionChannel::ECC_WorldDynamic, Params, HitResult))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to trace raycast from mouse position to world"));
        return false;
    }

    if (!HitResult.bBlockingHit)
    {
        return false;
    }

    outWorldPosition = HitResult.Location;
    return true;
}

TArray<ASelectableActor*> UStrategySelectionComponent::MakeSingleSelection()
{
    TArray<AActor*> outActors;
    TArray<ASelectableActor*> selectableActors;

    FVector WorldPosition;
    if (!DeprojectPositionToWorld(SelectionEndPoint, WorldPosition)) return selectableActors;
    
    // Check overlapping actors to selection sphere
    TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
    traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
    
    UClass* selectableClass = ASelectableActor::StaticClass();
    const TArray<AActor*> ignoreActors;
    
    // DrawDebugSphere(World, HitResult.Location, MaxSelectionApprox, 12, FColor::Red, true, 15.0f);
    UKismetSystemLibrary::SphereOverlapActors(this, WorldPosition, MaxSelectionApprox, traceObjectTypes, selectableClass, ignoreActors, outActors);

    for (auto a : outActors)
    {
        selectableActors.Add(Cast<ASelectableActor>(a));
    }
    
    return selectableActors;
}

TArray<ASelectableActor*> UStrategySelectionComponent::MakeMultipleSelection()
{
    TArray<ASelectableActor*> outActors;
    if (!ensure(PlayerController != nullptr)) return outActors;
    if (!ensure(HUD != nullptr)) return outActors;
   
    // Select all actors inside frustum
    outActors = HUD->GetCurrentSelection();
    return outActors;
}

void UStrategySelectionComponent::ResetSelection()
{
    for (auto actor : SelectedActors)
    {
        actor->SetActorSelected(false);
    }
    SelectedActors.Empty();
}

void UStrategySelectionComponent::SelectActors(TArray<ASelectableActor*> selectedActors)
{
    for (auto actor : selectedActors)
    {
        ASelectableActor* SelectableActor = Cast<ASelectableActor>(actor);
        SelectedActors.Add(SelectableActor);
        SelectableActor->SetActorSelected(true);
    }
}
