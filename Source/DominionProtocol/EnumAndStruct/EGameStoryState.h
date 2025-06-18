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
	Talk_Helper1 = 3 UMETA(DisplayName = "Talk_Helper1 - 조력자와 대화1"),
	Find_Boss1 = 4  UMETA(DisplayName = "Find_Boss1 - 보스1 탐색중"),
	Battle_Boss1 = 5  UMETA(DisplayName = "Battle_Boss1 - 보스1 전투"),
	Clear_Boss1 = 6  UMETA(DisplayName = "Clear_Boss1 - 보스1 종료"),
	Talk_Helper2 = 7  UMETA(DisplayName = "Talk_Helper2 - 조력자와 대화2"),
	Open_Path1 = 8  UMETA(DisplayName = "Open_Path1 - 열쇠 사용, 막힌길1 해금"),
	Find_Boss2 = 9  UMETA(DisplayName = "Find_Boss2 - 보스2 탐색중"),
	Battle_Boss2 = 10  UMETA(DisplayName = "Battle_Boss2 - 보스2 전투"),
	Clear_Boss2 = 11 UMETA(DisplayName = "Clear_Boss2 - 보스2 종료"),
	Talk_Helper3 = 12 UMETA(DisplayName = "Talk_Helper3 - 조력자와 대화3"),
	Open_Path2 = 13 UMETA(DisplayName = "Open_Path2 - 에너지코어 사용, 막힌길2 해금"),
	Open_Lever = 14 UMETA(DisplayName = "Open_Lever - 레버 해금"),
	Find_Boss3 = 15 UMETA(DisplayName = "Find_Boss3 - 보스3 탐색중"),
	Battle_Boss3 = 16 UMETA(DisplayName = "Battle_Boss3 - 보스3 전투"),
	Clear_Boss3 = 17 UMETA(DisplayName = "Clear_Boss3 - 보스3 종료"),
	Talk_Helper4 = 18 UMETA(DisplayName = "Talk_Helper4 - 조력자와 대화4"),
	Open_Path3 = 19 UMETA(DisplayName = "Open_Path3 - 막힌길3 해금"),
	Find_Boss4 = 20 UMETA(DisplayName = "Find_Boss4 - 보스4 탐색중"),
	Battle_Boss4 = 21 UMETA(DisplayName = "Battle_Boss4 - 보스4 전투"),
	Clear_Boss4 = 22 UMETA(DisplayName = "Clear_Boss4 - 보스4 종료"),
	Talk_Witch = 23 UMETA(DisplayName = "Talk_Witch - 마녀와 대화"),
	Find_Boss5 = 24 UMETA(DisplayName = "Find_Boss5 - 최종보스 탐색중"),
	Battle_Boss5 = 25 UMETA(DisplayName = "Battle_Boss5 - 최종보스 전투"),
	Clear_Boss5 = 26 UMETA(DisplayName = "Clear_Boss5 - 최종보스 종료"),
	Game_End = 27 UMETA(DisplayName = "Game_End - 게임 종료")
};