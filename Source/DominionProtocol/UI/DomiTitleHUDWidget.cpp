#include "DomiTitleHUDWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UDomiTitleHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ensure(LoadButton);
    ensure(NewGameButton);
    ensure(ExitButton);

    LoadButton->OnClicked.AddDynamic(this, &UDomiTitleHUDWidget::OnLoadClicked);
    NewGameButton->OnClicked.AddDynamic(this, &UDomiTitleHUDWidget::OnNewGameClicked);
    ExitButton->OnClicked.AddDynamic(this, &UDomiTitleHUDWidget::OnExitClicked);
}

void UDomiTitleHUDWidget::OnLoadClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Load clicked"));
    // 예: 저장된 게임 로드
}

void UDomiTitleHUDWidget::OnNewGameClicked()
{
    UE_LOG(LogTemp, Log, TEXT("New Game clicked"));
    UGameplayStatics::OpenLevel(this, FName("Lobby")); // 첫 레벨로 전환
}

void UDomiTitleHUDWidget::OnExitClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Exit clicked"));
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}