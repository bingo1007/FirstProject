// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ArduinoSerialSubsystem.generated.h"

struct FArduinoSerialPortData;  //handle 구조체 전방선언

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnParcelDetected);

UCLASS()
class FIRSTPROJECT_API UArduinoSerialSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	FArduinoSerialPortData* SerialPortData = nullptr;  //COM 포트의 핸들을 보관하는 구조체를 가리키는 포인터 변수. 포인터를 사용하는 이유는 헤더 파일에서 handle타입을 직접 사용할 필요가 없기 때문
public:
	UPROPERTY(BlueprintAssignable, Category = "Arduino")
	FOnParcelDetected OnParcelDetected;

	UFUNCTION(BlueprintCallable, Category = "Arduino")
	void SimulateDetected();

	UFUNCTION(BlueprintCallable, Category = "Arduino")
	void ProcessSerialMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "Arduino")
	bool OpenSerialPort(const FString& PortName);

	UFUNCTION(BlueprintCallable, Category = "Arduino")
	void CloseSerialPort();

	virtual void Deinitialize() override;
	
};
