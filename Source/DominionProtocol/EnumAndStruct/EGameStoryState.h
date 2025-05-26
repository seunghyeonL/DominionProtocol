// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EGameStoryState.generated.h"

UENUM(BlueprintType)
enum class EGameStoryState : uint8
{
	Tutorial= 0 UMETA(DisplayName = "Tutorial"),
	TalkWithHelper1 = 1 UMETA(DisplayName = "TalkWithHelper_1"),
	BossDoorOpened = 2 UMETA(DisplayName = "FirstBossDoorOpened"),
	
};

/*
	디폴트
	튜토리얼(게임에 대한 설명) -> (시작지점으로 워프)
	조력자와 대화 1. 세계관 설명, 균열에 대한 설명
	막힌 길1 조우
	과거로 이동 보스 조우
	보스 처치, 조력자와 대화
	막힌 길2 조우
	막힌 길 1 해금
	보스 2 처치, 조력자와 대화
	막힌 길 3 조우
	보스 3-1 처치
	보스 3-2 처치, 마녀와 대화
	최종 보스 레벨 이동
	최종 보스 처치


*/
