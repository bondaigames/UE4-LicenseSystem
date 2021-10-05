// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SaveData.h"
#include "LSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LICENSESYSTEM_API ULSGameInstance : public UGameInstance
{
	GENERATED_BODY()
private:
	static const FString UNIQUE_SLOT_NAME;
	static const uint32 USER_INDEX;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USaveData* CurrentSaveData;
	
	UFUNCTION()
	bool IsNewGame();
	UFUNCTION(BlueprintCallable)
	bool CreateNewSaveGame();
	UFUNCTION(BlueprintCallable)
	bool SaveGame();
	UFUNCTION(BlueprintCallable)
	bool LoadGame();
	UFUNCTION(BlueprintCallable)
	bool IsNetworkConnected();
};
