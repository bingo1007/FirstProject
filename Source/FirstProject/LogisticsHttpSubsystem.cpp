// Fill out your copyright notice in the Description page of Project Settings.
//언리얼 엔진은 HTTP 요청을 만들어 서버로 보내고, 서버는 해당 요청을 처리한다.

#include "LogisticsHttpSubsystem.h"
#include "HttpModule.h"  //unreal의 Http모듈에 접근
#include "Interfaces/IHttpRequest.h"  //Http 객체를 다룸
#include "Interfaces/IHttpResponse.h"  //서버에 반환하는 Http응답을 다룬다.
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"  //JSON 구조 객체 -> JSON으로 바꾸는 직렬화를 위한 헤더 파일
#include "Serialization/JsonWriter.h"  //JSON데이터를 만들기 위한 헤더 파일


void ULogisticsHttpSubsystem::SendParcelCreated(int32 ParcelId, FString Destination)  //ASP.Net core의 Post를 사용하도록 요청하는 함수.
{
	//이 함수가 호출될때마다 새로운 Http 객체가 생성이되고, 그 객체를 Request라는 이름의 스마트 포인터를 통해 참조하여 실행. 즉 택배의 갯수에 따라 함수가 호출되고 그럴때마다 새로운 객체 생성.
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =   //생성된 객체를 관리할 스마트 포인터의 자료형
		FHttpModule::Get().CreateRequest();  //unreal의 Http모듈에 접근해서 새로운 Http 요청 객체를 생성. 이 객체를 사용하여 앞으로 서버주소, HTTP메서드, Json 데이터 등을 설정.
	
	Request->SetURL("http://localhost:5141/api/Parcel");  //Http 요청을 보낼 서버 주소
	Request->SetVerb("POST");  //메서드 지정
	Request->SetHeader("Content-Type", "application/json");  //Http 요청의 추가 정보 설정 => Content-Type은 헤더의 한 영역으로 요청 본문에 담긴 데이터 형식이 JSON이라는 사실 전달.
	
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();  //TSharedRef와 비슷한 스마트 포인터. 다만 null값을 허용함. 객체가 없을 수도 있는 경우 사용.
	
	JsonObject->SetNumberField("parcelId", ParcelId);  //만든 JSON객체에 ParcelID와 목적지 기입.
	JsonObject->SetStringField("destination", Destination);  //다만 아직 Json 문자열로 직렬화한 것이 아닌, 메모리에 객체를 구상한 정도.
	//지금의 과정은 JSON 객체 메모리에 구상 -> 구상된 데이터로 다시 JSON데이터 생성이라는 다소 번거로운 과정을 거치고 있다. 바로 JSON 데이터를 만들면 될 것을 왜 굳이 이렇게 할까?
	//데이터를 만드는 과정에 있어서 위의 과정을 직렬화라고 하는데 직렬화 과정을 거치면서 복잡한 데이터나 잘못된 데이터를 일차적으로 수정할 수 있기 때문이다.

	FString JsonString;  //직렬화 결과를 저장할 문자열
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);  //Json텍스트를 작성할 Writer생성
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);  //Json객체를 문자열로 직렬화
	Request->SetContentAsString(JsonString);  //생성한 문자열을 HTTP 요청 본문에 설정

	Request->OnProcessRequestComplete().BindLambda(  //HTTP요청이 완료되면 실행할 람다함수 = 콜백함수.
		[](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bWasSuccessful)  //콜백의 매개변수, 요청과 응답 그리고 결과를 bool값으로 받음
		{
			if (bWasSuccessful && HttpResponse.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("[Parcel] HTTP Status: %d"),
					HttpResponse->GetResponseCode());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[Parcel] HTTP Request Failed"));
			}
		}
	);


	Request->ProcessRequest();  //드디어!! HTTP 요청 시작!! 단 서버 응답 확인은 또 따로 적성해야 됨ㅋ + 블루프린트에서 연결하지 않았으니 요청도 안됨
}

void ULogisticsHttpSubsystem::SendParcelEvent(int32 ParcelId, FString EventType, FString Destination)
{
	//이 함수가 호출될때마다 새로운 Http 객체가 생성이되고, 그 객체를 Request라는 이름의 스마트 포인터를 통해 참조하여 실행. 즉 택배의 갯수에 따라 함수가 호출되고 그럴때마다 새로운 객체 생성.
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =   //생성된 객체를 관리할 스마트 포인터의 자료형
		FHttpModule::Get().CreateRequest();  //unreal의 Http모듈에 접근해서 새로운 Http 요청 객체를 생성. 이 객체를 사용하여 앞으로 서버주소, HTTP메서드, Json 데이터 등을 설정.

	Request->SetURL("http://localhost:5141/api/ParcelEvent");  //Http 요청을 보낼 서버 주소
	Request->SetVerb("POST");  //메서드 지정
	Request->SetHeader("Content-Type", "application/json");  //Http 요청의 추가 정보 설정 => Content-Type은 헤더의 한 영역으로 요청 본문에 담긴 데이터 형식이 JSON이라는 사실 전달.

	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();  //TSharedRef와 비슷한 스마트 포인터. 다만 null값을 허용함. 객체가 없을 수도 있는 경우 사용.

	JsonObject->SetNumberField("parcelId", ParcelId);  //만든 JSON객체에 ParcelID와 목적지 기입.
	JsonObject->SetStringField("eventType", EventType);
	JsonObject->SetStringField("destination", Destination);  //다만 아직 Json 문자열로 직렬화한 것이 아닌, 메모리에 객체를 구상한 정도.
	//지금의 과정은 JSON 객체 메모리에 구상 -> 구상된 데이터로 다시 JSON데이터 생성이라는 다소 번거로운 과정을 거치고 있다. 바로 JSON 데이터를 만들면 될 것을 왜 굳이 이렇게 할까?
	//데이터를 만드는 과정에 있어서 위의 과정을 직렬화라고 하는데 직렬화 과정을 거치면서 복잡한 데이터나 잘못된 데이터를 일차적으로 수정할 수 있기 때문이다.

	FString JsonString;  //직렬화 결과를 저장할 문자열
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);  //Json텍스트를 작성할 Writer생성
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);  //Json객체를 문자열로 직렬화
	Request->SetContentAsString(JsonString);  //생성한 문자열을 HTTP 요청 본문에 설정

	Request->OnProcessRequestComplete().BindLambda(  //HTTP요청이 완료되면 실행할 람다함수 = 콜백함수.
		[](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bWasSuccessful)  //콜백의 매개변수, 요청과 응답 그리고 결과를 bool값으로 받음
		{
			if (bWasSuccessful && HttpResponse.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("[ParcelEvent] HTTP Status: %d"),
					HttpResponse->GetResponseCode());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[ParcelEvent] HTTP Request Failed"));
			}
		}
	);


	Request->ProcessRequest();  //드디어!! HTTP 요청 시작!! 단 서버 응답 확인은 또 따로 적성해야 됨ㅋ + 블루프린트에서 연결하지 않았으니 요청도 안됨
}

void ULogisticsHttpSubsystem::GetStatistics()
{
	//이 함수가 호출될때마다 새로운 Http 객체가 생성이되고, 그 객체를 Request라는 이름의 스마트 포인터를 통해 참조하여 실행.
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =   //생성된 객체를 관리할 스마트 포인터의 자료형
		FHttpModule::Get().CreateRequest();  //unreal의 Http모듈에 접근해서 새로운 Http 요청 객체를 생성. 이 객체를 사용하여 앞으로 서버주소, HTTP메서드, Json 데이터 등을 설정.

	Request->SetURL("http://localhost:5141/api/Statistics");  //Http 요청을 보낼 서버 주소
	Request->SetVerb("GET");  //메서드 지정

	TWeakObjectPtr<ULogisticsHttpSubsystem> WeakThis(this);  //TWeakObjectPtr는 Unreal의 UObject를 약하게 참조하는 스마트 포인터, 객체의 수명을 연장하지 않으면서, 아직 유효한지 확인

	Request->OnProcessRequestComplete().BindLambda(  //HTTP요청이 완료되면 실행할 람다함수 = 콜백함수.
		[WeakThis](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bWasSuccessful)  //콜백의 매개변수, 요청과 응답 그리고 결과를 bool값으로 받음
		{
			if (bWasSuccessful && HttpResponse.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("[Statistics] HTTP Status : %d"),
					HttpResponse->GetResponseCode());

				if (HttpResponse->GetResponseCode() == 200)
				{
					FString ResponseString = HttpResponse->GetContentAsString();  //HTTP 응답 본문을 string형태로 가져오는 함수. 즉 ResponseString에는 
					UE_LOG(LogTemp, Warning, TEXT("[Statistics] Response : %s"),
						*ResponseString);

					//역직렬화 과정. 언리얼 -> 서버로 POST를 실행할 땐 데이터를 JSON파일로 변환하는 직렬화를 거쳤는데 지금은 반대의 상황이니 JSON을 언리얼 변수로 만드는 역직렬화 과정 필요.
					TSharedPtr<FJsonObject> JsonObject;  //JSON 객체를 참조할 스마트 포인터

					TSharedRef<TJsonReader<>> Reader =  //문자열을 읽을 JSON Reader 생성
						TJsonReaderFactory<>::Create(ResponseString);

					if (FJsonSerializer::Deserialize(Reader, JsonObject)  //JSON 문자열을 객체로 변환
						&& JsonObject.IsValid())  //변환된 객체가 유효한지 확인
					{
						if (!WeakThis.IsValid())
						{
							return;
						}

						//지정한 키에 해당하는 정수 값 추출.
						ULogisticsHttpSubsystem* Subsystem = WeakThis.Get();

						Subsystem->TotalParcels =
							JsonObject->GetIntegerField(TEXT("totalParcels"));

						Subsystem->ScannedEvents =
							JsonObject->GetIntegerField(TEXT("scannedEvents"));

						Subsystem->SortedEvents =
							JsonObject->GetIntegerField(TEXT("sortedEvents"));

						Subsystem->OnStatisticsUpdated.Broadcast(
							Subsystem->TotalParcels,
							Subsystem->ScannedEvents,
							Subsystem->SortedEvents
						);

						UE_LOG(LogTemp, Warning,
							TEXT("[Statistics] Saved - Total: %d, Scanned: %d, Sorted: %d"),
							Subsystem->TotalParcels,
							Subsystem->ScannedEvents,
							Subsystem->SortedEvents);
					}
					else
					{
						UE_LOG(LogTemp, Error,
							TEXT("[Statistics] JSON Deserialization Failed"));
					}
				}
				
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[Statistics] Statistics Request Failed"));
			}
		}
	);

	Request->ProcessRequest();  //드디어!! HTTP 요청 시작!! 단 서버 응답 확인은 또 따로 적성해야 됨ㅋ + 블루프린트에서 연결하지 않았으니 요청도 안됨
}

void ULogisticsHttpSubsystem::GetRecentEvents()
{
	//이 함수가 호출될때마다 새로운 Http 객체가 생성이되고, 그 객체를 Request라는 이름의 스마트 포인터를 통해 참조하여 실행.
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =   //생성된 객체를 관리할 스마트 포인터의 자료형
		FHttpModule::Get().CreateRequest();  //unreal의 Http모듈에 접근해서 새로운 Http 요청 객체를 생성. 이 객체를 사용하여 앞으로 서버주소, HTTP메서드, Json 데이터 등을 설정.

	Request->SetURL("http://localhost:5141/api/ParcelEvent/recent");  //Http 요청을 보낼 서버 주소
	Request->SetVerb("GET");  //메서드 지정

	Request->OnProcessRequestComplete().BindLambda(  //HTTP요청이 완료되면 실행할 람다함수 = 콜백함수.
		[](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bWasSuccessful)  //콜백의 매개변수, 요청과 응답 그리고 결과를 bool값으로 받음
		{
			if (bWasSuccessful && HttpResponse.IsValid() && HttpResponse->GetResponseCode()==200)
			{
				FString ResponseString = HttpResponse->GetContentAsString();  //HTTP 응답 본문을 string형태로 가져오는 함수. 
				UE_LOG(LogTemp, Warning, TEXT("[Statistics] Response : %s"),
					*ResponseString);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[Statistics/Recent] Statistics Recent Request Failed"));
			}
		}
	);

	Request->ProcessRequest();  //드디어!! HTTP 요청 시작!! 단 서버 응답 확인은 또 따로 적성해야 됨ㅋ + 블루프린트에서 연결하지 않았으니 요청도 안됨

	UE_LOG(LogTemp, Warning, TEXT("GetRecentEvents Called"));
}
