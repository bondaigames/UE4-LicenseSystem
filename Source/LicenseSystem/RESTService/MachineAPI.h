// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HTTPService.h"
#include "MachineAPI.generated.h"

UENUM()
enum class EStatus : uint8
{
	Created     UMETA(DisplayName = "Created"),
	Deleted      UMETA(DisplayName = "Deleted"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMachineDelegate, EStatus, type, bool, success);

USTRUCT()
struct FMachineLicenseType
{
	GENERATED_BODY()
	/*UPROPERTY()
	FString type;*/
	UPROPERTY()
	FString type;
	UPROPERTY()
	FString id;
};

USTRUCT()
struct FMachineLicense
{
	GENERATED_BODY()
	UPROPERTY()
	FMachineLicenseType data;
};

USTRUCT()
struct FMachineRelationships
{
	GENERATED_BODY()
	UPROPERTY()
	FMachineLicense license;
};

USTRUCT()
struct FMachineAttributes
{
	GENERATED_BODY()
	UPROPERTY()
	FString fingerprint;
	UPROPERTY()
	FString platform;
	UPROPERTY()
	FString name;
};

USTRUCT()
struct FMachineData
{
	GENERATED_BODY()
	UPROPERTY()
	FString type;
	UPROPERTY()
	FMachineAttributes attributes;
	UPROPERTY()
	FMachineRelationships relationships;
};

USTRUCT()
struct FMachineRequest
{
	GENERATED_BODY()
	UPROPERTY()
	FMachineData data;
};

USTRUCT()
struct FMachineDataRes
{
	GENERATED_BODY()
        
	UPROPERTY()
	FString id;
};

USTRUCT()
struct FMachineResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FMachineDataRes data;
};

/**
 * 
 */
UCLASS()
class LICENSESYSTEM_API AMachineAPI : public AHTTPService
{
	GENERATED_BODY()
private:
	static const FString ACTIVE_MACHINE_API;
public:
	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FMachineDelegate MachineDelegate;
	void DeleteMachine(FString MachineId);
	void ActiveMachine(FMachineRequest data, FString token);

	void DeleteMachineResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void ActiveMachineResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
};
