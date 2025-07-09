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

// 추가 : DebugLine 콘솔 변수 설정

#include "DevCheatManager.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

namespace Debug
{
	inline TAutoConsoleVariable<int32> CVarShowDebugLines(
	TEXT("Debug.ShowLines"),
	0,
	TEXT("디버그라인 표시 제어 : 0 = 숨김, 1 = 표시"),
	ECVF_Default);

	inline TAutoConsoleVariable<bool> CVarEnableDebugPrint(
		TEXT("Debug.ToggleLogPrint"),
		false,
		TEXT("Enable or disable Debug::Log print globally"),
		ECVF_Default);
	
	static bool IsDebugPrintEnable()
	{
		return CVarEnableDebugPrint.GetValueOnAnyThread();
	}
	
	//로그와 스크린디버그메시지 동시 출력
	static void Print(const FString& Msg, const FColor& Color = FColor::Green, int32 Inkey = -1)
	{
		if (!IsDebugPrintEnable())
		{
			return;
		}
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Inkey, 7.f, Color, Msg);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}

	//로그만 출력(Warning)
	static void PrintLog(const FString& Msg)
	{
		if (!IsDebugPrintEnable())
		{
			return;
		}
		
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	}

	//로그만 출력(Error)
	static void PrintError(const FString& Msg)
	{
		if (!IsDebugPrintEnable())
		{
			return;
		}
		
		UE_LOG(LogTemp, Error, TEXT("%s"), *Msg);
	}

	//스크린디버그메시지만 출력
	static void PrintDM(const FString& Msg, const FColor& Color = FColor::Green, int32 Inkey = -1)
	{
		if (!IsDebugPrintEnable())
		{
			return;
		}
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Inkey, 5.f, Color, Msg);
		}
	}

	static bool ShouldShowDebugLines()
	{
		return CVarShowDebugLines.GetValueOnAnyThread() != 0;
	}

	static void ToggleDebugLines()
	{
		int32 CurrentValue = CVarShowDebugLines.GetValueOnAnyThread();
		IConsoleManager::Get().FindConsoleVariable(TEXT("Debug.ShowLines"))->Set(CurrentValue == 0 ? 1 : 0);

		FString Message = FString::Printf(TEXT("Debug Lines : %s"), CurrentValue == 0 ? TEXT("On") : TEXT("Off"));
		PrintDM(Message, FColor::Purple);
	}

	// 디버그 라인
	static void DrawLine(const UWorld* World, const FVector& LineStart, const FVector& LineEnd,
	                     const FColor& Color = FColor::Red, bool bPersistentLines = false, float Duration = -1.f,
	                     uint8 Depth = 0, float Thickness = 1.0f
	)
	{
		if (ShouldShowDebugLines())
		{
			::DrawDebugLine(World, LineStart, LineEnd, Color, bPersistentLines, Duration, Depth, Thickness);
		}
	}

	// 디버그 구체
	static void DrawSphere(const UWorld* World, const FVector& Center, float Radius = 50.f,
	                       int32 Segments = 12, const FColor& Color = FColor::Red, bool bPersistentLines = false,
	                       float Duration = -1.f, uint8 Depth = 0, float Thickness = 1.f)
	{
		if (ShouldShowDebugLines())
		{
			::DrawDebugSphere(World, Center, Radius, Segments, Color, bPersistentLines, Duration, Depth, Thickness);
		}
	}

	// 디버그 박스
	static void DrawBox(const UWorld* World, const FVector& Center, const FVector& Extent,
	                    const FColor& Color = FColor::Red, bool bPersistentLines = false, float Duration = -1.f,
	                    uint8 Depth = 0, float Thickness = 1.f
	)
	{
		if (ShouldShowDebugLines())
		{
			::DrawDebugBox(World, Center, Extent, Color, bPersistentLines, Duration, Depth, Thickness);
		}
	}

	// 디버그 캡슐
	static void DrawCapsule(const UWorld* World, const FVector& Center, float HalfHeight, float Radius,
	                        const FQuat& Rotation, const FColor& Color = FColor::Red, bool bPersistentLines = false,
	                        float Duration = -1.f, uint8 Depth = 0, float Thickness = 1.f)
	{
		if (ShouldShowDebugLines())
		{
			::DrawDebugCapsule(World, Center, HalfHeight, Radius, Rotation, Color, bPersistentLines, Duration, Depth, Thickness);
		}
	}
}
