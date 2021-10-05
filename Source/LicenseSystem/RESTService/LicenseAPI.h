// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTTPService.h"
#include "LicenseAPI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseDelegate, FString, licenseId);

USTRUCT()
struct FMetaDataTypeRes
{
	GENERATED_BODY()
	UPROPERTY()
	bool valid;
	UPROPERTY()
	FString constant;
};
USTRUCT()
struct FMetaDataType
{
	GENERATED_BODY()
	UPROPERTY()
	FString key;
};

USTRUCT()
struct FValidateData
{
	GENERATED_BODY()
	UPROPERTY()
	FString id;
	UPROPERTY()
	FString type;
};

USTRUCT()
struct FValidateRequest
{
	GENERATED_BODY()
	UPROPERTY()
	FMetaDataType meta;
};

USTRUCT()
struct FValidateResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FMetaDataTypeRes meta;
	UPROPERTY()
	FValidateData data;
};

/**
 * 
 */
UCLASS()
class LICENSESYSTEM_API ALicenseAPI : public AHTTPService
{
	GENERATED_BODY()
private:
	static const FString LICENSE_API;

public:
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FResponseDelegate ResponseDelegate;
	
	/** API REQUEST */
	void ValidateKey(FValidateRequest data);
	void RetrieveLicense(FString LicenseId);

	/** LIST DATA RESPONSE */
	void ValidateKeyResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void RetrieveLicenseResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};


