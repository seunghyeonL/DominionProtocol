// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BaseHUDWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UUserWidget* UBaseHUDWidget::PushUI(UUserWidget* WidgetToPush)
{
	// 0. 설정 체크
	if (!RootCanvasPanel || !IsValid(WidgetToPush))
	{
		return nullptr;	
	}
	
	const UPanelWidget* ParentPanel = WidgetToPush->GetParent();
	if (!ParentPanel || Cast<UCanvasPanel>(ParentPanel) != RootCanvasPanel)
	{
		return nullptr;
	}

	// 1. 활성화 목록에 이미 존재할 경우, Stack 순서 조정 및 ZOrder 업데이트
	const int32 ExistingWidgetIndex = ActivatedUIStack.Find(WidgetToPush);
	if (ExistingWidgetIndex != INDEX_NONE)
	{
		ActivatedUIStack.Remove(WidgetToPush);
		ActivatedUIStack.Add(WidgetToPush);
		// OnCurrentTopUIChanged Delegate
		OnCurrentTopUIChanged.Broadcast(WidgetToPush);
		
		UpdateAllWidgetZOrderInStack();
		WidgetToPush->SetVisibility(ESlateVisibility::Visible);
		SetInputModeForUIStack();
		
		return WidgetToPush;
	}

	// 2-1. 활성화 목록에 존재하지 않으므로, 이전 TopUI 를 숨김 처리 
	if (ActivatedUIStack.Num() > 0)
	{
		UUserWidget* CurrentTopUI = ActivatedUIStack.Last();
		if (CurrentTopUI && CurrentTopUI->GetParent() == RootCanvasPanel)
		{
			CurrentTopUI->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	// 2-2. 활성화 목록에 위젯 추가 및 활성화 설정
	ActivatedUIStack.Add(WidgetToPush);
	// OnCurrentTopUIChanged Delegate
	OnCurrentTopUIChanged.Broadcast(WidgetToPush);
	
	WidgetToPush->SetVisibility(ESlateVisibility::Visible);
	UpdateAllWidgetZOrderInStack();
	SetInputModeForUIStack();

	return WidgetToPush;
}

void UBaseHUDWidget::PopUI()
{
	if (ActivatedUIStack.Num() == 0)
	{
		return;
	}

	TObjectPtr<UUserWidget> CurrentTopUI = ActivatedUIStack.Last();
	if (CurrentTopUI && CurrentTopUI->GetParent() == RootCanvasPanel)
	{
		CurrentTopUI->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ActivatedUIStack.Num() > 0)
	{
		TObjectPtr<UUserWidget> NewTopUI = ActivatedUIStack.Last();
		if (NewTopUI && NewTopUI->GetParent() == RootCanvasPanel)
		{
			NewTopUI->SetVisibility(ESlateVisibility::Visible);
			// OnCurrentTopUIChanged Delegate
			OnCurrentTopUIChanged.Broadcast(NewTopUI);
		}
	}

	UpdateAllWidgetZOrderInStack();
	SetInputModeForUIStack();
}

void UBaseHUDWidget::PopSpecificUI(UUserWidget* WidgetToPop)
{
	if (!IsValid(WidgetToPop))
	{
		return;
	}

	const int32 FoundWidgetIndex = ActivatedUIStack.Find(WidgetToPop);
	if (FoundWidgetIndex != INDEX_NONE)
	{
		// 1. 최상위 여부 확인
		bool bWasTopUI = false;
		if (FoundWidgetIndex == (ActivatedUIStack.Num() - 1))
		{
			bWasTopUI = true;	
		}

		// 2. 해당 위젯 스택에서 삭제 
		if (WidgetToPop->GetParent() == RootCanvasPanel)
		{
			WidgetToPop->SetVisibility(ESlateVisibility::Collapsed);
		}
		ActivatedUIStack.Remove(WidgetToPop);

		// 3. Pop 한 Widget 이 과거 최상위 위젯이었고, 스택에 위젯이 남아있다면 새로운 최상위 위젯 표시
		if (bWasTopUI && ActivatedUIStack.Num() > 0)
		{
			TObjectPtr<UUserWidget> NewTopUI = ActivatedUIStack.Last();
			if (NewTopUI && NewTopUI->GetParent() == RootCanvasPanel)
			{
				NewTopUI->SetVisibility(ESlateVisibility::Visible);
				// OnCurrentTopUIChanged Delegate
				OnCurrentTopUIChanged.Broadcast(NewTopUI);
			}
		}
		
		UpdateAllWidgetZOrderInStack();
		SetInputModeForUIStack();
	}
}

void UBaseHUDWidget::UpdateAllWidgetZOrderInStack()
{
	for (int32 i = 0; i < ActivatedUIStack.Num(); i++)
	{
		const TObjectPtr<UUserWidget> Widget = ActivatedUIStack[i];
		if (Widget && Widget->GetParent() == RootCanvasPanel)
		{
			auto* CanvasSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
			if (CanvasSlot)
			{
				CanvasSlot->SetZOrder(i);
			}
		}
	}
}

void UBaseHUDWidget::ClearUIStack()
{
	for (int32 i = 0; i < ActivatedUIStack.Num(); i++)
	{
		// 가장 위에 존재하는 UI 부터 차례대로 비활성화
		const TObjectPtr<UUserWidget> Widget = ActivatedUIStack[ActivatedUIStack.Num() - 1 - i];
		if (Widget && Widget->GetParent() == RootCanvasPanel)
		{
			Widget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	ActivatedUIStack.Empty();

	// OnCurrentTopUIChanged Delegate
	OnCurrentTopUIChanged.Broadcast(nullptr);
	SetInputModeForUIStack();
}

void UBaseHUDWidget::SetInputModeForUIStack()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController Not Found"));
		return;
	}

	// 활성화 된 UI 가 없다면 Game 을 위한 InputMode 설정, 있다면 UI 를 위한 InputMode 설정
	if (ActivatedUIStack.Num() > 0)
	{
		const FInputModeGameAndUI InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
	else
	{
		const FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}
}

void UBaseHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensureMsgf(RootCanvasPanel, TEXT("RootCanvasPanel Not Connected"));
}
