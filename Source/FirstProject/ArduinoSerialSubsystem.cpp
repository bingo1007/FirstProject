// Fill out your copyright notice in the Description page of Project Settings.


#include "ArduinoSerialSubsystem.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
struct FArduinoSerialPortData  //전역 변수로 handle 구조체 생성 -> 이래야 함수에서 받은 데이터를 저장하여 계속 사용할 수 있음. 일종의 캐싱 과정.
{
	HANDLE Handle = INVALID_HANDLE_VALUE;
};
#endif

void UArduinoSerialSubsystem::SimulateDetected()  //아무런 조건 없이 탐지. 탐지 후 브로드 캐스팅
{
	OnParcelDetected.Broadcast();
}

void UArduinoSerialSubsystem::ProcessSerialMessage(const FString& Message)  //메세지 비교 후 DETECT라면 탐지.
{
	if (Message == TEXT("DETECT"))
	{
		OnParcelDetected.Broadcast();
	}
}

bool UArduinoSerialSubsystem::OpenSerialPort(const FString& PortName)  //아두이노와 연결 된 포트를 여는 함수
{
#if PLATFORM_WINDOWS
	if (SerialPortData == nullptr)  //포트 구조체 포인터가 아무것도 없다면, 새로운 구조체로 초기화
	{
		SerialPortData = new FArduinoSerialPortData();
	}
	if (SerialPortData->Handle != INVALID_HANDLE_VALUE)  //이미 있는 구조체라면 에러 출력. 즉 중복 접근 방지
	{
		UE_LOG(LogTemp, Error, TEXT("Already open serial port : %s"), *PortName);
		return false;
	}

	FString PortPath = TEXT("\\\\.\\") + PortName;  //포트 주소 저장

	SerialPortData->Handle = CreateFileW(  //포트를 열고 HANDLE 객체 할당
		*PortPath,
		GENERIC_READ | GENERIC_WRITE,
		0,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
	);

	if (SerialPortData->Handle == INVALID_HANDLE_VALUE)  //포트가 열리지 않으면 에러 출력
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to open serial port : %s"), *PortName);
		return false;
	}

	DCB SerialParams = {};  //아두이노의 serial통신은 직렬 통신 장치. 이 직렬 통신 장치와 동일한 통신 속도와 데이터 형식을 사용해야 하는데 이를 설정하기 위한 구조체인 DCB 구조체 생성 및 초기화
	SerialParams.DCBlength = sizeof(DCB);  //Windows API에 DCB구조체의 크기를 알려준다. 

	if(!GetCommState(SerialPortData->Handle, &SerialParams)) //COM 포트에 설정된 현재 통신 정보를 DCB 구조체에 저장하는 함수, handle과 dcb구조체의 주소를 인자로 받는다.
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get serial port settings."));

		CloseSerialPort();
		return false;
	}

	//SerialParams 구조체 내부의 설정값 변경
	SerialParams.BaudRate = CBR_9600;
	SerialParams.ByteSize = 8;
	SerialParams.Parity = NOPARITY;
	SerialParams.StopBits = ONESTOPBIT;

	if (!SetCommState(SerialPortData->Handle, &SerialParams)) //DCB 구조체에 저장된 통신 설정을 실제 COM 포트에 적용하는 함수
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to set serial port settings."));

		CloseSerialPort();
		return false;
	}

	COMMTIMEOUTS Timeouts = {};  //Serial 통신의 읽기 및 쓰기 타임아웃을 설정하는 Windows 구조체
	//수신 데이터가 없으면 즉시 반환하도록 설정
	Timeouts.ReadIntervalTimeout = MAXDWORD;
	Timeouts.ReadTotalTimeoutMultiplier = 0;
	Timeouts.ReadTotalTimeoutConstant = 0;
	Timeouts.WriteTotalTimeoutMultiplier = 0;
	Timeouts.WriteTotalTimeoutConstant = 100;

	if (!SetCommTimeouts(SerialPortData->Handle, &Timeouts))  //timeouts 설정값 변경 및 오류 처리
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to set Timeouts Settings."));

		CloseSerialPort();
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Serial port opened : %s"), *PortName);  //Windows API와 약속된 형식에 맞게 구조체의 크기 정보를 명시적으로 기록해야 함
	return true;
#else
	UE_LOG(LogTemp, Warning, TEXT("Serial communication is only supported on windows."));
	return false;
#endif
}

void UArduinoSerialSubsystem::CloseSerialPort()
{
#if PLATFORM_WINDOWS
	if (SerialPortData != nullptr)
	{
		if (SerialPortData->Handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(SerialPortData->Handle);
		}
		delete SerialPortData;
		SerialPortData = nullptr;
	}
#endif
}

void UArduinoSerialSubsystem::Deinitialize()  //이미 만들어진 Deinitialize에 serial port 데이터 정리만 추가. 즉 기존 삭제함수 + 포트 데이터 삭제 동시에 함.
{
	CloseSerialPort();
	Super::Deinitialize();
}
