// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LogisticsHttpSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(  //정수 값 세 개를 전달할 수 있는 이벤트 타입을 선언
	FOnStatisticsUpdated,
	int32, TotalParcels,
	int32, ScannedEvents,
	int32, SortedEvents
);

UCLASS()
class FIRSTPROJECT_API ULogisticsHttpSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//택배 생성시 객체 생성 및 추가 함수
	UFUNCTION(BlueprintCallable, Category = "Logistics|HTTP")  //unreal의 c++함수를 블루프린트로 호출 가능
	void SendParcelCreated(int32 ParcelId, FString Destination);  //기존에 우리는 parcelid와 destination을 json형태로 post요청. 이 기능을 블프 함수로 만든 것일 뿐.

	//이벤트 발생시 객체 생성 및 추가 함수
	UFUNCTION(BlueprintCallable, Category = "Logistics|HTTP")  
	void SendParcelEvent(int32 ParcelId, FString EventType, FString Destination);

	//STATISTICS 요청 함수
	UFUNCTION(BlueprintCallable, Category = "Logistics|HTTP")
	void GetStatistics();

	//최근 5개 로그 요청 함수
	UFUNCTION(BlueprintCallable, Category = "Logistics|HTTP")
	void GetRecentEvents();

	//Statistics 받아오면 저장할 변수들, 이거 하면 블프에서도 사용 가능. 즉 이것들은 서브 시스템의 변수들임.
	UPROPERTY(BlueprintReadOnly, Category = "Logistics|Statistics")
	int32 TotalParcels = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Logistics|Statistics")
	int32 ScannedEvents = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Logistics|Statistics")
	int32 SortedEvents = 0;

	UPROPERTY(BlueprintAssignable, Category = "Logistics|Statistics")  //BlueprintAssignable은 Blueprint에서 해당 이벤트에 바인딩할 수 있도록 한다.
	FOnStatisticsUpdated OnStatisticsUpdated;
};
