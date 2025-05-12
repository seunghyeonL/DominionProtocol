#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiTitleHUDWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiTitleHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UDomiMainMenuWidget> MainMenu;

};