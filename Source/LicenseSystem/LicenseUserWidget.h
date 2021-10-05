// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RESTService/MachineAPI.h"
#include "LicenseUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LICENSESYSTEM_API ULicenseUserWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	UFUNCTION()
	void ResponseData(FString LicenseId);
	UFUNCTION()
	void MachineResponseData(EStatus type, bool success);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATokenAPI> TokenRequest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ALicenseAPI> LicenseRequest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AMachineAPI> MachineRequest;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FString FormatGeneratedKey(const FString& key);
	UFUNCTION(BlueprintImplementableEvent)
	void ShowErrorMessage(const FString& message);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatedUI(EStatus type, bool success);
	
	UFUNCTION(BlueprintCallable)
	bool IsFirstActivation();
	UFUNCTION(BlueprintCallable)
	FString GenerateKeyString(const int32 Length);
	UFUNCTION(BlueprintCallable)
	void ClipboardCopy(FString Str);
	UFUNCTION(BlueprintCallable)
	void GetAuthToken();
	UFUNCTION(BlueprintCallable)
	void ValidLicenseKey();
	UFUNCTION(BlueprintCallable)
	void CreateNewMachine();
	UFUNCTION(BlueprintCallable)
	void DeleteMachine();
	
	UFUNCTION(BlueprintCallable)
	void Testing();
	void LoginResponse();
};
