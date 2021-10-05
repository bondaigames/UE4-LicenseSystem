// Fill out your copyright notice in the Description page of Project Settings.


#include "TokenAPI.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
#include "../LSGameInstance.h"

void ATokenAPI::GetAuthToken()
{
	//"/v1/accounts/aa76c17f-ba51-4766-9b76-27edae2e3a4c/tokens"
	FString URL = FString::Printf(TEXT("/v1/accounts/%s/tokens"), *ACCOUNT_ID);
	UE_LOG(LogTemp, Warning, TEXT("URL: %s"), *URL);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = PostRequest(URL, "");
	FString val = "Basic " + FBase64::Encode("testing@gmail.com:123456");
	Request->SetHeader(TEXT("Authorization"), val);
	Request->OnProcessRequestComplete().BindUObject(this, &ATokenAPI::TokenResponse);
	Send(Request);
}

void ATokenAPI::TokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful)) {
		return;
	}
	//FString JsonString = Response->GetContentAsString();
	//UE_LOG(LogTemp, Warning, TEXT("json: %s"), *JsonString);
	/*FAuthTokenResponse AuthTokenRes;
	GetStructFromJsonString<FAuthTokenResponse>(Response, AuthTokenRes);*/

	FAuthTokenResponse AuthTokenRes;
	FString JsonStr = Response->GetContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("json: %s"), *JsonStr);
	FJsonObjectConverter::JsonObjectStringToUStruct<FAuthTokenResponse>(JsonStr, &AuthTokenRes, 0, 0);

	UE_LOG(LogTemp, Warning, TEXT("token id: %s"), *AuthTokenRes.data.attributes.token);

	FString AuthToken = AuthTokenRes.data.attributes.token;

	if (AuthToken.Len() > 0)
	{
		UWorld* World = GetWorld();
		ULSGameInstance* GameInstance = World ? World->GetGameInstance<ULSGameInstance>() : nullptr;
		GameInstance->CurrentSaveData->AuthToken = AuthTokenRes.data.attributes.token;
		GameInstance->CurrentSaveData->AuthTokenExpire = AuthTokenRes.data.attributes.expiry;//FDateTime::Now() + FTimespan(14, 0, 0, 0);
		GameInstance->SaveGame();
		UE_LOG(LogTemp, Warning, TEXT("date time expire: %s"), *GameInstance->CurrentSaveData->AuthTokenExpire.ToString());
		Destroy();
	}
}
