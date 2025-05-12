// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiMainMenuWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;


protected:
	// 위젯 바인딩용 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> StartGameButton;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> OptionButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UButton> ExitButton;

};
