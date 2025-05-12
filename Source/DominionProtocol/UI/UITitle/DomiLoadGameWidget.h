#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiLoadGameWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiLoadGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	// SaveSlotEntryClass(단일 슬롯)이 담길 슬롯 리스트
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SaveSlotList;

	UPROPERTY(EditAnywhere, Category = "SaveLoad")
	TSubclassOf<UUserWidget> SaveSlotEntryClass; // 예시

public:
	// 저장 슬롯 리스트를 다시 채우는 함수
	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void RefreshSaveSlots();

	// 슬롯 버튼이 클릭됐을 때 호출
	UFUNCTION()
	void OnSelectSaveSlot(const FString& SlotName);
};
