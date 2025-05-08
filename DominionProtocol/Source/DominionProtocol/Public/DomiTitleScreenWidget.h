#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiTitleScreenWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiTitleScreenWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // 위젯 바인딩용 변수
    UPROPERTY(meta = (BindWidget))
    class UButton* LoadButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* NewGameButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExitButton;

public:
    // 버튼에 연결될 함수들
    UFUNCTION()
    void OnLoadClicked();

    UFUNCTION()
    void OnNewGameClicked();

    UFUNCTION()
    void OnExitClicked();
};