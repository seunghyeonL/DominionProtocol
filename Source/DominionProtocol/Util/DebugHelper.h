#pragma once

//사용 예시
// #include "DebugHelper.h"  //사용할 코드 파일 위치에서 헤더파일 포함

// FString Msg = FString::Printf(출력할 포맷 설정해서);
// 예시 : FString Msg = FString::Printf(TEXT("PlayerLocation.X : %f"). PlayerLocation.X);
// Debug::Print(Msg);  요렇게도 가능합니다~!

// Debug::Print(TEXT("출력할 메시지"));
// Debug::PrintLog(TEXT("출력할 메시지"));
// Debug::PrintError(TEXT("출력할 메시지"));
// Debug::PrintDM(TEXT("출력할 메시지"));

//스크린 디버그 메시지에 색 지정 or Key값 필요한 경우
// Debug::Print(TEXT("출력할 메시지"), FColor::원하는색상);
// Debug::Print(TEXT("출력할 메시지"), FColor::원하는색상, 1);
// Debug::PrintDM(TEXT("출력할 메시지"), FColor::원하는색상);
// Debug::PrintDM(TEXT("출력할 메시지"), FColor::원하는색상, 1);

//왜 쓰냐?
//매번 UE_LOG(LogTemp, ~~~~~~) 쓰기 귀찮잖아요
// GEngine->AddOnScreenDebugMessage(~~~~~~~~~~~~) 도 그렇고
namespace Debug
{
	//로그와 스크린디버그메시지 동시 출력
	static void Print(const FString& Msg, const FColor& Color = FColor::Green, int32 Inkey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Inkey, 7.f, Color, Msg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}

	//로그만 출력(Warning)
	static void PrintLog(const FString& Msg)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	}

	//로그만 출력(Error)
	static void PrintError(const FString& Msg)
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *Msg);
	}

	//스크린디버그메시지만 출력
	static void PrintDM(const FString& Msg, const FColor& Color = FColor::Green, int32 Inkey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Inkey, 5.f, Color, Msg);
		}
	}
}