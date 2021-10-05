// Fill out your copyright notice in the Description page of Project Settings.


#include "MachineAPI.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "LicenseSystem/LSGameInstance.h"

const FString AMachineAPI::ACTIVE_MACHINE_API = FString::Printf(TEXT("/v1/accounts/%s/machines"), *ACCOUNT_ID);

void AMachineAPI::DeleteMachine(FString MachineId)
{
	UWorld* World = GetWorld();
	ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;

	FString URL = ACTIVE_MACHINE_API + "/" + MachineId;

	UE_LOG(LogTemp, Warning, TEXT("URL: %s"), *URL);
	
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = DeleteRequest(URL, "");
	FString val = "Bearer " + GameInstance->CurrentSaveData->AuthToken;
	UE_LOG(LogTemp, Warning, TEXT("token: %s"), *val);
	Request->SetHeader(TEXT("Authorization"), val);
	Request->OnProcessRequestComplete().BindUObject(this, &AMachineAPI::DeleteMachineResponse);
	Send(Request);
}

void AMachineAPI::ActiveMachine(FMachineRequest data, FString token)
{
	//FString URL = FString::Printf(TEXT("/v1/accounts/%s/machines"), *ACCOUNT_ID);
	FString URL = ACTIVE_MACHINE_API;
	UE_LOG(LogTemp, Warning, TEXT("URL: %s"), *URL);
	UE_LOG(LogTemp, Warning, TEXT("Token: %s"), *token);
	FString ContentJsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FMachineRequest::StaticStruct(), &data, ContentJsonString, 0, 0);
	ContentJsonString.ReplaceInline(TEXT("iD"), TEXT("id"), ESearchCase::IgnoreCase);
	//ContentJsonString.ReplaceInline(TEXT("ID"), TEXT("Id"), ESearchCase::CaseSensitive)
	UE_LOG(LogTemp, Warning, TEXT("content json: %s"), *ContentJsonString);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = PostRequest(URL, ContentJsonString);
	FString val = "Bearer " + token;
	Request->SetHeader(TEXT("Authorization"), val);
	Request->OnProcessRequestComplete().BindUObject(this, &AMachineAPI::ActiveMachineResponse);
	Send(Request);
}

void AMachineAPI::DeleteMachineResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) {
		return;
	}
	FString JsonString = Response->GetContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("json machine: %s"), *JsonString);
	if (Response->GetResponseCode() == 204)
	{
		UE_LOG(LogTemp, Warning, TEXT("delete machine success"));
		UWorld* World = GetWorld();
		ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;
		GameInstance->CurrentSaveData->MachineId = "";
		GameInstance->CurrentSaveData->IsActivated = false;
		GameInstance->SaveGame();
		MachineDelegate.Broadcast(EStatus::Deleted, true);
	}
	else
	{
		MachineDelegate.Broadcast(EStatus::Deleted, false);
	}
	
	Destroy();
}

void AMachineAPI::ActiveMachineResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) {
		return;
	}
	FString JsonString = Response->GetContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("json machine: %s"), *JsonString);
	/*FAuthTokenResponse AuthTokenRes;
	GetStructFromJsonString<FAuthTokenResponse>(Response, AuthTokenRes);*/

	FMachineResponse MachineRes;
	FString JsonStr = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<FMachineResponse>(JsonStr, &MachineRes, 0, 0);
	FString MachineId = MachineRes.data.id;
	UE_LOG(LogTemp, Warning, TEXT("machine id: %s"), *MachineId);
	if (MachineId.Len() > 0)
	{
		UWorld* World = GetWorld();
		ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;
		GameInstance->CurrentSaveData->MachineId = MachineId;
		GameInstance->SaveGame();
		MachineDelegate.Broadcast(EStatus::Created, true);
	}
	else
	{
		MachineDelegate.Broadcast(EStatus::Created, false);
	}
	Destroy();
}
