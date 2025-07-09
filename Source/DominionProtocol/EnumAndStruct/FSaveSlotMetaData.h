// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSaveSlotMetaData.generated.h"

USTRUCT(BlueprintType)
struct FSaveSlotMetaData
{
	GENERATED_BODY()

	// UI에서 필요. 표시 X. 이 값으로 빈 슬롯인지, 세이브 데이터 존재하는지 여부 판단하게 하면 됩니다.
	UPROPERTY(BlueprintReadOnly)
	bool SaveSlotExist;
	
	UPROPERTY(BlueprintReadOnly)
	FString SaveSlotName;

	// UI에서 필요. 표시 ?. 슬롯 순서 결정 위함(1, 2, 3 입니다. 0, 1, 2 아님)
	UPROPERTY(BlueprintReadOnly)
	int32 SaveSlotIndex;

	// UI에서 필요. 표시 O. 저장 날짜(xxxx.xx.xx~~)
	UPROPERTY(BlueprintReadOnly)
	FDateTime SaveDateTime;

	// UI에서 필요. 표시 O. 총 플레이 시간
	UPROPERTY(BlueprintReadOnly)
	int32 PlayTime;
	
	UPROPERTY(BlueprintReadOnly)
	FString PlayingLevelName;

	// UI에서 필요. 표시 O. 현재 플레이어가 위치한 레벨 이름
	UPROPERTY(BlueprintReadOnly)
	FText PlayingLevelDisplayName;

	// UI에서 필요. 표시 O. 가장 최근 균열(로드 시 여기서 시작함)
	UPROPERTY(BlueprintReadOnly)
	FText RecentCrackName;

	// UI에서 필요. 표시 O. 플레이어의 레벨(Lv)
	UPROPERTY(BlueprintReadOnly)
	int32 PlayerLevel;

	FSaveSlotMetaData()
		:	SaveSlotExist(false),
			SaveSlotName(""),
			SaveSlotIndex(-1),
			SaveDateTime(FDateTime::MinValue()),
			PlayTime(0),
			PlayingLevelName(TEXT("-")),
			PlayingLevelDisplayName(FText::FromString(TEXT("-"))),
			RecentCrackName(FText::FromString(TEXT("-"))),
			PlayerLevel(0)
	{}
};