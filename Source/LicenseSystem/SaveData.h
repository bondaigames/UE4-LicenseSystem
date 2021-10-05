// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveData.generated.h"

/**
 * 
 */
UCLASS()
class LICENSESYSTEM_API USaveData : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString LicenseServerIDString;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString LicenseString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool IsActivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString MachineId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString AuthToken;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FDateTime AuthTokenExpire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString SlotNameString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 UserIndex;

	void CreateSlot(const FString& SlotName, const int32& UserID);
};
