// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LogisticsHttpSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API ULogisticsHttpSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Logistics|HTTP")  //unreal의 c++함수를 블루프린트로 호출 가능
	void SendParcelCreated(int32 ParcelId, FString Destination);  //기존에 우리는 parcelid와 destination을 json형태로 post요청. 이 기능을 블프 함수로 만든 것일 뿐.
	
};
