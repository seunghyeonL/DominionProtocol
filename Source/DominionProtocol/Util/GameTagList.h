#pragma once

#include "GameplayTagContainer.h"

namespace EffectTags
{
	// ControlEffectTags
	inline const FGameplayTag UsingSkill = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill"));
	inline const FGameplayTag Death = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Death"));
	inline const FGameplayTag UsingDash = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill.Dash"));
	inline const FGameplayTag UsingParry = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill.Parry"));
	inline const FGameplayTag LockOn = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.LockOn"));
	
	inline const FGameplayTag Stiffness = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Stiffness"));
	inline const FGameplayTag Stun = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Stun"));
	inline const FGameplayTag Silence = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Silence"));
	inline const FGameplayTag Confused = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Confused"));
	inline const FGameplayTag Groggy = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Groggy"));

	// StatusEffectTags
	inline const FGameplayTag Running = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Stat.Running"));
	inline const FGameplayTag Slow = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Stat.Slow"));
	inline const FGameplayTag AttackDown = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Stat.AttackDown"));
	inline const FGameplayTag Burn = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.DoT.Burn"));
	inline const FGameplayTag Poison = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.DoT.Poison"));
}

namespace StatTags
{
	// BaseStatTags
	inline const FGameplayTag LIFE = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.LIFE"));
	inline const FGameplayTag STR = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.STR"));
	inline const FGameplayTag DEX = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.DEX"));

	// BattleStatTags
	inline const FGameplayTag MaxHealth = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.MaxHealth"));
	inline const FGameplayTag MaxStamina = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.MaxStamina"));
	inline const FGameplayTag MaxShield = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.MaxShield"));
	inline const FGameplayTag MaxGroggyGauge = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.MaxGroggyGauge"));
	inline const FGameplayTag AttackPower = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.AttackPower"));
	inline const FGameplayTag Defense = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.Defense"));
	inline const FGameplayTag MoveSpeed = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.MoveSpeed"));
	
	// VariableStatTags
	inline const FGameplayTag Health = FGameplayTag::RequestGameplayTag(TEXT("Stat.Variable.Health"));
	inline const FGameplayTag Stamina = FGameplayTag::RequestGameplayTag(TEXT("Stat.Variable.Stamina"));
	inline const FGameplayTag Shield = FGameplayTag::RequestGameplayTag(TEXT("Stat.Variable.Shield"));
	inline const FGameplayTag GroggyGauge = FGameplayTag::RequestGameplayTag(TEXT("Stat.Variable.GroggyGauge"));
}

namespace SkillGroupTags
{
	inline const FGameplayTag BaseAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack"));
	inline const FGameplayTag Dash = FGameplayTag::RequestGameplayTag(TEXT("Skill.Dash"));
}

namespace WeaponSkillGroupTags
{
	// inline const FGameplayTag Sword = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.Sword"));
}

namespace SkillTags
{
	inline const FGameplayTag BaseAttackSword1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.first.Sword"));
	inline const FGameplayTag BaseAttackSword2 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.second.Sword"));
	inline const FGameplayTag BaseAttackSword3 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.third.Sword"));

	inline const FGameplayTag BaseAttackSpear1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.first.Spear"));
	inline const FGameplayTag PlayerDash = FGameplayTag::RequestGameplayTag(TEXT("Skill.Dash.Player"));
	inline const FGameplayTag PlayerParry = FGameplayTag::RequestGameplayTag(TEXT("Skill.Parry.Player"));
	
}

//namespace EnemeyTags
//{
//	// PatternTags
//	inline const FGameplayTag Pattern1 = FGameplayTag::RequestGameplayTag(TEXT("AI.Base.Pattern1"));
//	inline const FGameplayTag SpecialPattern1 = FGameplayTag::RequestGameplayTag(TEXT("AI.Special.Pattern1"));
//}

namespace ItemTags
{
	inline const FGameplayTag ItemBase = FGameplayTag::RequestGameplayTag(TEXT("Item.Base"));

	//Weapon Item Tags
	inline const FGameplayTag BasicWeapon = FGameplayTag::RequestGameplayTag(TEXT("Item.Weapon.Basic"));

	//Consumable Item Tags
	inline const FGameplayTag Potion = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.Potion"));
	inline const FGameplayTag Elixir = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.Elixir"));
}