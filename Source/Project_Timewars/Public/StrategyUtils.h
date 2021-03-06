// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StrategyUtils.generated.h"

UENUM()
namespace ETeam {
	enum Type
	{
		Neutral UMETA(DisplayName = "Neutral"),
		Zombie UMETA(DisplayNam = "Zombie"),
		Survivor UMETA(DisplayName = "Survivor"),
	};
}

UENUM()
namespace ESurvivorColor {
	enum Type
	{
		Brown UMETA(DisplayName = "Team_White", Tooltip = "RESERVED for neutrals"),
		Red UMETA(DisplayName = "Team_Red", Tooltip = "RESERVED for zombies"),
        Blue UMETA(DisplayName = "Team_Blue"),
        Cyan UMETA(DisplayName = "Team_Cyan"),
        Green UMETA(DisplayName = "Team_Green"),
        Purple UMETA(DisplayName = "Team_Purple"),
    };
}

UENUM()
namespace EActorType
{
	enum Type
	{
		Unit,
	    Building,
	    Resource,
	    UNDEFINED
	};	
}

USTRUCT(BlueprintType)
struct FSelectableData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category=Data)
	TEnumAsByte<EActorType::Type> ActorType;

	UPROPERTY(EditDefaultsOnly, Category=Data)
	TEnumAsByte<ETeam::Type> OwningTeam;
	
	/** minimal attack damage */
    UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 AttackMin;

	/** maximum attack damage */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 AttackMax;

	/** minimal attack damage - siege */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 AttackMinSiege;
	
	/** maximum attack damage - siege */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 AttackMaxSiege;

	/** range of pawn attack */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 AttackDistance;

	/** damage reduction */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 DamageReduction;

	/** maximum health */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    float MaxHealth;

	/** current health */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    float Health;

	/** health change every 5 seconds */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    float HealthRegen;

	/** default base movement speed in km/h */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 MaxSpeed;

	/** current base movement speed in km/h, not considering modifiers */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 Speed;

	/** turning speed in radians/s */
	UPROPERTY(EditDefaultsOnly, Category=Data)
	FRotator MaxTurningSpeed;

	/** turning speed in radians/s */
	UPROPERTY(EditDefaultsOnly, Category=Data)
	FRotator TurningSpeed;

	/** defaults */
	FSelectableData()
	{
		ActorType = EActorType::UNDEFINED;
		OwningTeam = ETeam::Neutral;

		AttackMin = 5.f;
		AttackMax = 10.f;
		AttackMinSiege = 2.f;
		AttackMaxSiege = 5.f;
		AttackDistance = 100;
		DamageReduction = 0.f;

		MaxHealth = 100.f;
		Health = MaxHealth;
		HealthRegen = 1.f;

		MaxSpeed = 10;
		Speed = MaxSpeed;
		MaxTurningSpeed = FRotator(6, 6, 6);	
		TurningSpeed = MaxTurningSpeed;	
	}
};
