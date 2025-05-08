#include "DomiTitleScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UDomiTitleScreenWidget::NativeConstruct()
{
    Super::NativeConstruct();

    ensure(LoadButton);
    ensure(NewGameButton);
    ensure(ExitButton);

    LoadButton->OnClicked.AddDynamic(this, &UDomiTitleScreenWidget::OnLoadClicked);
    NewGameButton->OnClicked.AddDynamic(this, &UDomiTitleScreenWidget::OnNewGameClicked);
    ExitButton->OnClicked.AddDynamic(this, &UDomiTitleScreenWidget::OnExitClicked);
}

void UDomiTitleScreenWidget::OnLoadClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Load clicked"));
    // 예: 저장된 게임 로드
}

void UDomiTitleScreenWidget::OnNewGameClicked()
{
    UE_LOG(LogTemp, Log, TEXT("New Game clicked"));
    UGameplayStatics::OpenLevel(this, FName("Lobby")); // 첫 레벨로 전환
}

void UDomiTitleScreenWidget::OnExitClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Exit clicked"));
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}