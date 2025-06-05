// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EGameStoryState.generated.h"

UENUM(BlueprintType)
enum class EGameStoryState : uint8
{
	Tutorial = 0  UMETA(DisplayName = "튜토리얼 시작"),
	Game_Start = 1  UMETA(DisplayName = "스토리 시작"),
	Find_Path1 = 2  UMETA(DisplayName = "막힌길1 조우, 조력자와 대화1"),
	Find_Boss1 = 3  UMETA(DisplayName = "보스1 탐색중"),
	Battle_Boss1 = 4  UMETA(DisplayName = "보스1 전투"),
	Clear_Boss1 = 5  UMETA(DisplayName = "보스1 종료"),
	Talk_Helper2 = 6  UMETA(DisplayName = "조력자와 대화2"),
	Open_Path1 = 7  UMETA(DisplayName = "열쇠 사용, 막힌길1 해금"),
	Find_Boss2 = 8  UMETA(DisplayName = "보스2 탐색중"),
	Battle_Boss2 = 9  UMETA(DisplayName = "보스2 전투"),
	Clear_Boss2 = 10 UMETA(DisplayName = "보스2 종료"),
	Talk_Helper3 = 11 UMETA(DisplayName = "조력자와 대화3"),
	Open_Path2 = 12 UMETA(DisplayName = "에너지코어 사용, 막힌길2 해금"),
	Find_Note = 13 UMETA(DisplayName = "쪽지(과거2→3)"),
	Find_Path3 = 14 UMETA(DisplayName = "막힌길 3조우"),
	Find_Boss3_1 = 15 UMETA(DisplayName = "보스3 탐색중"),
	Battle_Boss3_1 = 16 UMETA(DisplayName = "보스3-1 전투"),
	Clear_Boss3_1 = 17 UMETA(DisplayName = "보스3-1 종료"),
	Talk_Helper4 = 18 UMETA(DisplayName = "조력자와 대화4"),
	Open_Path3 = 19 UMETA(DisplayName = "막힌길3 해금"),
	Find_Boss3_2 = 20 UMETA(DisplayName = "보스3-2 탐색중"),
	Battle_Boss3_2 = 21 UMETA(DisplayName = "보스3-2 전투"),
	Clear_Boss3_2 = 22 UMETA(DisplayName = "보스3-2 종료"),
	Talk_Witch = 23 UMETA(DisplayName = "마녀와 대화"),
	Find_Boss4 = 24 UMETA(DisplayName = "최종보스 탐색중"),
	Battle_Boss4 = 25 UMETA(DisplayName = "최종보스 전투"),
	Clear_Boss4 = 26 UMETA(DisplayName = "최종보스 종료"),
	Game_End = 27 UMETA(DisplayName = "게임 종료")
};