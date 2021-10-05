// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTTPService.h"
#include "TokenAPI.generated.h"

USTRUCT()
struct FAuthTokenAttributes
{
	GENERATED_BODY()
		UPROPERTY()
		FString kind;
	UPROPERTY()
		FString token;
	UPROPERTY()
		FDateTime expiry;
	UPROPERTY()
		FString created;
};

USTRUCT()
struct FAuthTokenData
{
	GENERATED_BODY()
		UPROPERTY()
		FString id;
	UPROPERTY()
		FString type;
	UPROPERTY()
		FAuthTokenAttributes attributes;

};

USTRUCT()
struct FAuthTokenResponse
{
	GENERATED_BODY()
		UPROPERTY()
		FAuthTokenData data;
};

UCLASS()
class LICENSESYSTEM_API ATokenAPI : public AHTTPService
{
	GENERATED_BODY()
public:
	void GetAuthToken();
	void TokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
