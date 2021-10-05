// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"

#include "HTTPService.generated.h"

UCLASS()
class LICENSESYSTEM_API AHTTPService : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHTTPService();
	
	
protected:
	static const FString ACCOUNT_ID;
	class FHttpModule* Http;
	FString APIBaseURL;
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> RequestWithRoute(FString Subroute);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> GetRequest(FString Subroute);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> PostRequest(FString Subroute, FString ContentJsonString);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> DeleteRequest(FString Subroute, FString ContentJsonString);
	void Send(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request);
	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);
	/*template <typename StructType>
	void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput);
	template <typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);*/
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/*void GetAuthToken();
	void TokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);*/
};
