// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIInGame/NewDialogueWidget.h"

#include "Kismet/GameplayStatics.h"
#include "WorldObjects/Crack.h"
#include "WorldObjects/StoryTrigger.h"
#include "WorldObjects/DialogueManager.h"

void UNewDialogueWidget::UpdateDialogueWidget(const FText NewText)
{
	CurrentDialogueText = NewText;
	OnUpdateDialogueWidget();
}

void UNewDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindCreateDialogueDelegate();
}

void UNewDialogueWidget::BindCreateDialogueDelegate()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACrack::StaticClass() ,Actors);

	for (const auto Actor : Actors)
	{
		auto* Crack = Cast<ACrack>(Actor);
		Crack->OnCreateDialogueManager.AddUObject(this, &UNewDialogueWidget::BindDialogueDelegate);
	}

	TArray<AActor*> TriggerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStoryTrigger::StaticClass(), TriggerActors);

	for (const auto Actor : TriggerActors)
	{
		auto* StoryTrigger = Cast<AStoryTrigger>(Actor);
		StoryTrigger->OnCreateDialogueManager.AddUObject(this, &UNewDialogueWidget::BindDialogueDelegate);
	}
}

void UNewDialogueWidget::BindDialogueDelegate(UDialogueManager* DialogueManager)
{
	CurrentDialogueManager = DialogueManager;
	DialogueManager->OnUpdateDialogueText.AddUObject(this, &UNewDialogueWidget::UpdateDialogueWidget);
}
