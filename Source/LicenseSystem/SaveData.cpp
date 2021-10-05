// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveData.h"

void USaveData::CreateSlot(const FString& SlotName, const int32& UserID)
{
    this->SlotNameString = SlotName;
    this->UserIndex = UserID;
    this->IsActivated = false;
}
