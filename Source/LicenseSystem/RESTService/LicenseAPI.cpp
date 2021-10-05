// Fill out your copyright notice in the Description page of Project Settings.


#include "LicenseAPI.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "LicenseSystem/LSGameInstance.h"

const FString ALicenseAPI::LICENSE_API = FString::Printf(TEXT("/v1/accounts/%s/licenses"), *ACCOUNT_ID);


void ALicenseAPI::ValidateKey(FValidateRequest data)
{
	UWorld* World = GetWorld();
	ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;

	FString URL = LICENSE_API + "/actions/validate-key";

	UE_LOG(LogTemp, Warning, TEXT("URL: %s"), *URL);
	FString ContentJsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FValidateRequest::StaticStruct(), &data, ContentJsonString, 0, 0);
	UE_LOG(LogTemp, Warning, TEXT("content json: %s"), *ContentJsonString);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = PostRequest(URL, ContentJsonString);
	Request->OnProcessRequestComplete().BindUObject(this, &ALicenseAPI::ValidateKeyResponse);
	Send(Request);
}

void ALicenseAPI::RetrieveLicense(FString LicenseId)
{
	UWorld* World = GetWorld();
	ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;
	
	FString URL = LICENSE_API + GameInstance->CurrentSaveData->LicenseServerIDString;
	
	UE_LOG(LogTemp, Warning, TEXT("URL: %s"), *URL);
	UE_LOG(LogTemp, Warning, TEXT("Token: %s"), *GameInstance->CurrentSaveData->AuthToken);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = GetRequest(URL);
	FString val = "Bearer " + GameInstance->CurrentSaveData->AuthToken;
	Request->SetHeader(TEXT("Authorization"), val);
	Request->OnProcessRequestComplete().BindUObject(this, &ALicenseAPI::RetrieveLicenseResponse);
	Send(Request);
}

void ALicenseAPI::ValidateKeyResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) {
		return;
	}
	
	FString JsonString = Response->GetContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("json machine: %s"), *JsonString);
	FValidateResponse ValidRes;
	FString JsonStr = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<FValidateResponse>(JsonStr, &ValidRes, 0, 0);
	UE_LOG(LogTemp, Warning, TEXT("validate id json: %s"), *ValidRes.data.id);
	FString LicenseId = ValidRes.data.id;
	if (LicenseId.Len() > 0)
	{
		UWorld* World = GetWorld();
		ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;
		GameInstance->CurrentSaveData->LicenseServerIDString = LicenseId;
		GameInstance->CurrentSaveData->IsActivated = ValidRes.meta.valid;
		GameInstance->SaveGame();
		UE_LOG(LogTemp, Warning, TEXT("testing aaa"), *GameInstance->CurrentSaveData->LicenseServerIDString);
	}
	ResponseDelegate.Broadcast(LicenseId);
	Destroy();
}

void ALicenseAPI::RetrieveLicenseResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) {
		return;
	}
	FString JsonString = Response->GetContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("json machine: %s"), *JsonString);
	//ResponseDelegate.Broadcast(JsonString);
}
