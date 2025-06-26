// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIInGame/NewDialogueWidget.h"

#include "Kismet/GameplayStatics.h"
#include "WorldObjects/Crack.h"
#include "WorldObjects/StoryTrigger.h"
#include "WorldObjects/BlockedPath.h"
#include "WorldObjects/DyingHelper.h"
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
	BindDialogueSources<ACrack>();
	BindDialogueSources<ABlockedPath>();
	BindDialogueSources<ADyingHelper>();
}

template<typename T>
void UNewDialogueWidget::BindDialogueSources()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), T::StaticClass(), Actors);

	for (const auto Actor : Actors)
	{
		if (T* TypedActor = Cast<T>(Actor))
		{
			TypedActor->OnCreateDialogueManager.AddUObject(this, &UNewDialogueWidget::BindDialogueDelegate);
			UE_LOG(LogTemp, Display, TEXT("UNewDialogueWidget::BindDialogueSources()"));
		}
	}
}

void UNewDialogueWidget::BindDialogueDelegate(UDialogueManager* DialogueManager)
{
	CurrentDialogueManager = DialogueManager;
	DialogueManager->OnUpdateDialogueText.Clear();
	DialogueManager->OnUpdateDialogueText.AddUObject(this, &UNewDialogueWidget::UpdateDialogueWidget);
}