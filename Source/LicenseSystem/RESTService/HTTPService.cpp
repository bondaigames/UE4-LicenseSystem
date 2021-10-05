// Fill out your copyright notice in the Description page of Project Settings.


#include "HTTPService.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/Base64.h"
#include "JsonObjectConverter.h"

const FString AHTTPService::ACCOUNT_ID = "aa76c17f-ba51-4766-9b76-27edae2e3a4c";

// Sets default values
AHTTPService::AHTTPService()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	APIBaseURL = TEXT("https://api.keygen.sh");

}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> AHTTPService::RequestWithRoute(FString Subroute)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(APIBaseURL + Subroute);
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
	return Request;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> AHTTPService::GetRequest(FString Subroute)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = RequestWithRoute(Subroute);
	Request->SetVerb("GET");
	return Request;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> AHTTPService::PostRequest(FString Subroute, FString ContentJsonString)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = RequestWithRoute(Subroute);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> AHTTPService::DeleteRequest(FString Subroute, FString ContentJsonString)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = RequestWithRoute(Subroute);
	Request->SetVerb("DELETE");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

void AHTTPService::Send(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request)
{
	Request->ProcessRequest();
}

bool AHTTPService::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful)
{
	if((!bWasSuccessful) || (!Response.IsValid())) 
	{
		return false;
	}
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) 
	{
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code:  %d"), Response->GetResponseCode());
	return false;
	
}
//
//template <typename StructType>
//void AHTTPService::GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput)
//{
//	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);
//}
//
//template <typename StructType>
//void AHTTPService::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput)
//{
//	StructType StructData;
//	FString JsonString = Response->GetContentAsString();
//	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
//}

// Called when the game starts or when spawned
void AHTTPService::BeginPlay()
{
	Super::BeginPlay();
	Http = &FHttpModule::Get();
}

// Called every frame
void AHTTPService::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void AHTTPService::GetAuthToken()
//{
//	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = PostRequest("/v1/accounts/aa76c17f-ba51-4766-9b76-27edae2e3a4c/tokens", "");
//	Request->OnProcessRequestComplete().BindUObject(this, &AHTTPService::TokenResponse);
//	Send(Request);
//}
//
//void AHTTPService::TokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
//{
//	if (!ResponseIsValid(Response, bWasSuccessful)) {
//		return;
//	}
//	FString JsonString = Response->GetContentAsString();
//	UE_LOG(LogTemp, Warning, TEXT("json: %s"), *JsonString);
//}


