// Fill out your copyright notice in the Description page of Project Settings.


#include "LSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "WinInet.h"

const FString ULSGameInstance::UNIQUE_SLOT_NAME = "Data_001";
const uint32 ULSGameInstance::USER_INDEX = 0;

bool ULSGameInstance::IsNewGame()
{
	if (UGameplayStatics::DoesSaveGameExist(UNIQUE_SLOT_NAME, USER_INDEX))
	{
		return false;
	}
	return true;
}

bool ULSGameInstance::CreateNewSaveGame()
{
	if (CurrentSaveData != nullptr)
	{
		CurrentSaveData->CreateSlot(UNIQUE_SLOT_NAME, USER_INDEX);
	}
	else
	{
		USaveData* SaveGameInstance = Cast<USaveData>(UGameplayStatics::CreateSaveGameObject(USaveData::StaticClass()));
		if (SaveGameInstance != nullptr)
		{
			CurrentSaveData = SaveGameInstance;
		}
	}
	return UGameplayStatics::SaveGameToSlot(CurrentSaveData, UNIQUE_SLOT_NAME, USER_INDEX);
}

bool ULSGameInstance::SaveGame()
{
	if (CurrentSaveData != nullptr)
	{
		return UGameplayStatics::SaveGameToSlot(CurrentSaveData, UNIQUE_SLOT_NAME, USER_INDEX);
	}
	return false;
}

bool ULSGameInstance::LoadGame()
{
	CurrentSaveData = nullptr;
	USaveData* LoadedGame = Cast<USaveData>(UGameplayStatics::LoadGameFromSlot(UNIQUE_SLOT_NAME, USER_INDEX));
	bool IsLoaded = false;
	if (LoadedGame != nullptr)
	{
		CurrentSaveData = LoadedGame;
		if (CurrentSaveData != nullptr)
		{
			// The operation was successful, so LoadedGame now contains the data we saved earlier.
			UE_LOG(LogTemp, Warning, TEXT("LOADED: %s"), *LoadedGame->LicenseString);
			IsLoaded = true;
		}
	}
	return IsLoaded;
}

bool ULSGameInstance::IsNetworkConnected()
{
	if (InternetCheckConnection(L"http://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0))
	{
		return true;
	}
	return false;
}
