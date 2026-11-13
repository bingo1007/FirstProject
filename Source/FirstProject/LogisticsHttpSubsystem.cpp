// Fill out your copyright notice in the Description page of Project Settings.
//언리얼 엔진은 HTTP 요청을 만들어 서버로 보내고, 서버는 해당 요청을 처리한다.

#include "HttpModule.h"  //unreal의 Http모듈에 접근
#include "Interfaces/IHttpRequest.h"  //Http 객체를 다룸
#include "Interfaces/IHttpResponse.h"  //서버에 반환하는 Http응답을 다룬다.
#include "LogisticsHttpSubsystem.h"

void ULogisticsHttpSubsystem::SendParcelCreated(int32 ParcelId, FString Destination)  //ASP.Net core의 Post를 사용하도록 요청하는 함수.
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =   //생성된 객체를 관리할 스마트 포인터의 자료형
		FHttpModule::Get().CreateRequest();  //unreal의 Http모듈에 접근해서 새로운 Http 요청 객체를 생성. 이 객체를 사용하여 앞으로 서버주소, HTTP메서드, Json 데이터 등을 설정.
}
