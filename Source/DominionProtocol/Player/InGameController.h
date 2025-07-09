// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerController.h"
#include "InGameController.generated.h"

DECLARE_DELEGATE(FOnPressedMainMenuSwitchShowAndHideWidgetEvent);
DECLARE_DELEGATE(FOnPressedDialogueChangedNextStoryState);

DECLARE_DELEGATE(FOnPressedCrackMenuBackButtonEvent);
DECLARE_DELEGATE(FOnPressedCrackMenuConfirmButtonEvent);

DECLARE_DELEGATE(FOnPressedMainMenuButtonQEvent);
DECLARE_DELEGATE(FOnPressedMainMenuButtonEEvent);
DECLARE_DELEGATE(FOnPressedMainMenuButtonREvent);
DECLARE_DELEGATE(FOnPressedMainMenuButtonAEvent);
DECLARE_DELEGATE(FOnPressedMainMenuButtonDEvent);
DECLARE_DELEGATE(FOnPressedMainMenuButtonZEvent);
DECLARE_DELEGATE(FOnPressedMainMenuButtonCEvent);
DECLARE_DELEGATE(FOnPressedMainMenuButtonSpaceBarEvent);

class UDomiInGameHUDWidget;

UCLASS()
class DOMINIONPROTOCOL_API AInGameController : public ABasePlayerController
{
	GENERATED_BODY()

public:
	FOnPressedMainMenuSwitchShowAndHideWidgetEvent OnPressedMainMenuSwitchShowAndHideWidgetEvent;
	FOnPressedDialogueChangedNextStoryState OnPressedDialogueChangedNextStoryState;
	FOnPressedCrackMenuBackButtonEvent OnPressedCrackMenuBackButtonEvent;
	FOnPressedCrackMenuConfirmButtonEvent OnPressedCrackMenuConfirmButtonEvent;
	FOnPressedMainMenuButtonQEvent OnPressedMainMenuButtonQEvent;
	FOnPressedMainMenuButtonEEvent OnPressedMainMenuButtonEEvent;
	FOnPressedMainMenuButtonREvent OnPressedMainMenuButtonREvent;
	FOnPressedMainMenuButtonAEvent OnPressedMainMenuButtonAEvent;
	FOnPressedMainMenuButtonDEvent OnPressedMainMenuButtonDEvent;
	FOnPressedMainMenuButtonZEvent OnPressedMainMenuButtonZEvent;
	FOnPressedMainMenuButtonCEvent OnPressedMainMenuButtonCEvent;
	FOnPressedMainMenuButtonSpaceBarEvent OnPressedMainMenuButtonSpaceBarEvent;
	
	//
	
	AInGameController();
	
	// 필요시 위젯에서 호출하여 사용
	UFUNCTION(BlueprintCallable)
	void SetupMappingContext(UInputMappingContext* NewMappingContext);
	
	// Binding InputAction
	UFUNCTION()
	void OnMainMenuSwitchShowAndHideWidget();

	UFUNCTION()
	void OnDialogueChangedNextStoryState();

	UFUNCTION()
	void OnPressedCrackMenuBackButton();

	UFUNCTION()
	void OnPressedCrackMenuConfirmButton();
	
	UFUNCTION()
	void OnPressedMainMenuButtonQ();
	
	UFUNCTION()
	void OnPressedMainMenuButtonE();
	
	UFUNCTION()
	void OnPressedMainMenuButtonR();
	
	UFUNCTION()
	void OnPressedMainMenuButtonA();
	
	UFUNCTION()
	void OnPressedMainMenuButtonD();
	
	UFUNCTION()
	void OnPressedMainMenuButtonZ();
	
	UFUNCTION()
	void OnPressedMainMenuButtonC();
	
	UFUNCTION()
	void OnPressedMainMenuButtonSpaceBar();
	
	UFUNCTION(BlueprintPure)
	UDomiInGameHUDWidget* GetInGameHUDWidget() const { return InGameHUDWidgetInstance; }

protected:
	virtual void BeginPlay() override;
	
	virtual void CreateAndAddHUDWidget() override;
	virtual void SetupInputMode() override;
	virtual void SetupMappingContext() override;
	virtual void BindInputActions() override;
	
public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MainMenuMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DialogueMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> CrackMenuMappingContext;



#pragma region Character Input Actions Section
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	// Dash Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DashAction;
	
	// Sprint Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	// Parry Input Action 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ParryAction;
	
	// BaseAttack Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> BaseAttackAction;

	// WeaponSkill Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> WeaponSkillAction;

	// MagicSkill Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MagicSkillAction;

	// Interact Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	// RockOn Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LockOnAction;

	//Consume slot 1 Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ConsumeItemAction_1;

	//Consume slot 2 Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ConsumeItemAction_2;

	//Consume slot 3 Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ConsumeItemAction_3;

	//Swap Weapon Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SwapWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractionScroll;

	
#pragma endregion
	
#pragma region Controller Input Actions Section
	// MainMenuUI Section 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MainMenuSwitchShowAndHideWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MainMenuPressButtonQ;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MainMenuPressButtonE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MainMenuPressButtonR;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MainMenuPressButtonA;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MainMenuPressButtonD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MainMenuPressButtonZ;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MainMenuPressButtonC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MainMenuPressButtonSpaceBar;
	
	
	
	
	// DialogueUI Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DialogueChangeNextStoryState;


	// CrackMenuUI Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PressedCrackBackButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PressedCrackConfirmButton;
	
#pragma endregion
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UDomiInGameHUDWidget> InGameHUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UDomiInGameHUDWidget> InGameHUDWidgetInstance;
	
	bool bActiveInGameMenuOpen = false;
};
