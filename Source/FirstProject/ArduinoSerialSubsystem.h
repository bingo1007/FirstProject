// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ArduinoSerialSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnParcelDetected);

UCLASS()
class FIRSTPROJECT_API UArduinoSerialSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = "Arduino")
	FOnParcelDetected OnParcelDetected;

	UFUNCTION(BlueprintCallable, Category = "Arduino")
	void SimulateDetected();

	UFUNCTION(BlueprintCallable, Category = "Arduino")
	void ProcessSerialMessage(const FString& Message);
	
};
