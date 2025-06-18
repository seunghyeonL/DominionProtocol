// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InGameController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class DOMINIONPROTOCOL_API AInGameController : public APlayerController
{
	GENERATED_BODY()

public:
	AInGameController();
	
	// 필요시 위젯에서 호출하여 사용)
	UFUNCTION(BlueprintCallable)
	void SetupMappingContext(class UInputMappingContext* NewMappingContext);
	
	void HandleSetupInGameHUD();
	
	void RemoveAllMappingContext();
	
	void OnMainMenuSwitchShowAndHideWidget();
	
	void OnDialogueChangedNextStoryState();

	UFUNCTION(BlueprintPure)
	class UDomiInGameHUDWidget* GetInGameHUDWidget() const { return InGameHUDWidgetInstance; }

protected:
	virtual void BeginPlay() override;

	void CreateHUDWidget();
	void AddHUDToViewport() const;

	void BindControllerInputActions();
	
public:
	
	/** MappingContext */
	UPROPERTY(BlueprintReadOnly)
	TArray<UInputMappingContext*> MappingContextArray;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MainMenuSwitchShowAndHideWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DialogueChangeNextStoryState;
	
#pragma endregion
	
protected:
	UPROPERTY()
	TObjectPtr<class UEnhancedInputLocalPlayerSubsystem> LocalPlayerInputSubsystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UDomiInGameHUDWidget> InGameHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<class UDomiInGameHUDWidget> InGameHUDWidgetInstance;

	bool bActiveInGameMenuOpen = false;
};
