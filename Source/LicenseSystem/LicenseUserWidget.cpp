// Fill out your copyright notice in the Description page of Project Settings.


#include "LicenseUserWidget.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "SaveData.h"
#include "LSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "RESTService/LicenseAPI.h"
#include "RESTService/TokenAPI.h"


void ULicenseUserWidget::ResponseData(FString LicenseId)
{
	UE_LOG(LogTemp, Warning, TEXT("license id: %s"), *LicenseId);
	if (LicenseId.Len() > 0)
	{
		CreateNewMachine();
	}
	else
	{
		ShowErrorMessage("Key does not find on server");
	}
	
}

void ULicenseUserWidget::MachineResponseData(EStatus type, bool success)
{
	UpdatedUI(type, success);
	/*if (type == EStatus::Created && success)
	{
		UE_LOG(LogTemp, Warning, TEXT("created success"));
	}
	else if (type == EStatus::Created && !success)
	{
		UE_LOG(LogTemp, Warning, TEXT("created error"));
	}
	else if (type == EStatus::Deleted && success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Deleted success"));
	}
	else if (type == EStatus::Deleted && !success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Deleted error"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(""));
	}*/
}

bool ULicenseUserWidget::IsFirstActivation()
{
	UWorld* World = GetWorld();
	ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;
	
	if (GameInstance->IsNewGame()) return true;
	return false;
}

FString ULicenseUserWidget::GenerateKeyString(const int32 Length)
{
	TArray<FString> AlphaNum = {	"0", "1", "2", "3", "4",
									"5", "6", "7", "8", "9",
									"A", "B", "C", "D", "E",
									"F", "G", "H", "I", "J",
									"K", "L", "M", "N", "O",
									"P", "Q", "R", "S", "T",
									"U", "V", "W", "X", "Y",
									"Z"
							 };
	FString KeyGen = TEXT("");
	for (int i = 0; i < Length; i++)
	{
		const int32 RandomIndex = FMath::RandRange(0, AlphaNum.Num() - 1);
		
		UE_LOG(LogTemp, Warning, TEXT("Random Character: %s"), *AlphaNum[RandomIndex]);
		KeyGen.Append(AlphaNum[RandomIndex]);
	}
	return KeyGen;
}

void ULicenseUserWidget::ClipboardCopy(FString Str)
{
	FGenericPlatformMisc::ClipboardCopy(*Str);
}

void ULicenseUserWidget::GetAuthToken()
{
	UWorld* World = GetWorld();
	ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;

	FDateTime CurrentDateTime = FDateTime::Now();

	FDateTime AuthTokenExpire = GameInstance->CurrentSaveData->AuthTokenExpire;
	UE_LOG(LogTemp, Warning, TEXT("AuthToken: %s"), *GameInstance->CurrentSaveData->AuthToken);
	UE_LOG(LogTemp, Warning, TEXT("AuthTokenExpire.ToString: %s"), *AuthTokenExpire.ToString());
	
	//Auth token expired then need to request the new one
	if (AuthTokenExpire < CurrentDateTime)
	{
		ATokenAPI* TokenAPI = World->SpawnActor<ATokenAPI>(TokenRequest);
		TokenAPI->GetAuthToken();
	}
}

void ULicenseUserWidget::ValidLicenseKey()
{
	UWorld* World = GetWorld();
	ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;

	FValidateRequest request;
	request.meta.key = FormatGeneratedKey(GameInstance->CurrentSaveData->LicenseString);
	
	ALicenseAPI* LicenseAPI = World->SpawnActor<ALicenseAPI>(LicenseRequest);
	LicenseAPI->ResponseDelegate.AddDynamic(this, &ULicenseUserWidget::ResponseData);
	LicenseAPI->ValidateKey(request);
	
}

//void ULicenseUserWidget::RetrieveLicense()
//{
	/*UWorld* World = GetWorld();
	ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;
	ALicenseAPI* LicenseAPI = World->SpawnActor<ALicenseAPI>(LicenseRequest);*/
	//LicenseAPI->RetrieveLicense()
//}

void ULicenseUserWidget::CreateNewMachine()
{
	UWorld* World = GetWorld();
	ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;
	FMachineRequest request;
	request.data.type = "machines";
	request.data.attributes.fingerprint = FormatGeneratedKey(GenerateKeyString(25));
	request.data.attributes.platform = UGameplayStatics::GetPlatformName();
	request.data.attributes.name = FGenericPlatformProcess::ComputerName();
	
	request.data.relationships.license.data.type = "licenses";
	FString License = GameInstance->CurrentSaveData->LicenseServerIDString;
	request.data.relationships.license.data.id = License;
	
	AMachineAPI* MachineAPI = World->SpawnActor<AMachineAPI>(MachineRequest);
	MachineAPI->MachineDelegate.AddDynamic(this, &ULicenseUserWidget::MachineResponseData);
	MachineAPI->ActiveMachine(request, GameInstance->CurrentSaveData->AuthToken);
}

void ULicenseUserWidget::DeleteMachine()
{
	UWorld* World = GetWorld();
	ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;
	FString MachineId = GameInstance->CurrentSaveData->MachineId;
	AMachineAPI* MachineAPI = World->SpawnActor<AMachineAPI>(MachineRequest);
	MachineAPI->MachineDelegate.AddDynamic(this, &ULicenseUserWidget::MachineResponseData);
	MachineAPI->DeleteMachine(MachineId);
}

void ULicenseUserWidget::Testing()
{
	//FString URL = FString::Printf(TEXT("/v1/accounts/%s/tokens"), ACCOUNT_ID);
	//UE_LOG(LogTemp, Warning, TEXT("URL: %s"), *URL);
	UWorld* World = GetWorld();
	ATokenAPI* TokenAPI = World->SpawnActor<ATokenAPI>(TokenRequest);
	TokenAPI->GetAuthToken();
}


