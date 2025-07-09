#include "DomiLoadGameWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"

void UDomiLoadGameWidget::NativeConstruct()
{
    Super::NativeConstruct();
    RefreshSaveSlots();
}

void UDomiLoadGameWidget::RefreshSaveSlots()
{
    
}

void UDomiLoadGameWidget::OnSelectSaveSlot(const FString& SlotName)
{
    
}