// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EGameStoryState.generated.h"

UENUM(BlueprintType)
enum class EGameStoryState : uint8
{
	Tutorial = 0  UMETA(DisplayName = "Tutorial - 튜토리얼 시작"),
	Game_Start = 1  UMETA(DisplayName = "Game_Start - 스토리 시작"),
	Find_Path1 = 2  UMETA(DisplayName = "Find_Path1 - 막힌길1 조우"),
	Find_Boss1 = 3  UMETA(DisplayName = "Find_Boss1 - 보스1 탐색중"),
	Battle_Boss1 = 4  UMETA(DisplayName = "Battle_Boss1 - 보스1 전투"),
	Clear_Boss1 = 5  UMETA(DisplayName = "Clear_Boss1 - 보스1 종료"),
	Open_Path1 = 6  UMETA(DisplayName = "Open_Path1 - 열쇠 사용, 막힌길1 해금"),
	Find_Boss2 = 7  UMETA(DisplayName = "Find_Boss2 - 보스2 탐색중"),
	Battle_Boss2 = 8  UMETA(DisplayName = "Battle_Boss2 - 보스2 전투"),
	Clear_Boss2 = 9 UMETA(DisplayName = "Clear_Boss2 - 보스2 종료"),
	Open_Path2 = 10 UMETA(DisplayName = "Open_Path2 - 에너지코어 사용, 막힌길2 해금"),
	Open_Lever = 11 UMETA(DisplayName = "Open_Lever - 레버 해금"),
	Find_Boss3 = 12 UMETA(DisplayName = "Find_Boss3 - 보스3 탐색중"),
	Battle_Boss3 = 13 UMETA(DisplayName = "Battle_Boss3 - 보스3 전투"),
	Clear_Boss3 = 14 UMETA(DisplayName = "Clear_Boss3 - 보스3 종료"),
	Find_Boss4 = 15 UMETA(DisplayName = "Find_Boss4 - 보스4 탐색중"),
	Battle_Boss4 = 16 UMETA(DisplayName = "Battle_Boss4 - 보스4 전투"),
	Find_Boss5 = 17 UMETA(DisplayName = "Find_Boss5 - 최종보스 탐색중"),
	Battle_Boss5 = 18 UMETA(DisplayName = "Battle_Boss5 - 최종보스 전투"),
	Game_End = 19 UMETA(DisplayName = "Game_End - 게임 종료")
};