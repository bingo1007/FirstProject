// Fill out your copyright notice in the Description page of Project Settings.


#include "ArduinoSerialSubsystem.h"

void UArduinoSerialSubsystem::SimulateDetected()
{
	OnParcelDetected.Broadcast();
}
