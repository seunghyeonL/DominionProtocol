#pragma once

#include "GameplayTagContainer.h"

namespace EffectTags
{
	// ControlEffectTags
	inline const FGameplayTag UsingSkill = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill"));
	inline const FGameplayTag Death = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Death"));
	inline const FGameplayTag UsingDash = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill.Dash"));
	inline const FGameplayTag UsingParry = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill.Parry"));
	inline const FGameplayTag UsingZoneya = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill.Zoneya"));
	inline const FGameplayTag LockOn = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.LockOn"));

	inline const FGameplayTag Stiffness = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Stiffness"));
	inline const FGameplayTag Stun = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Stun"));
	inline const FGameplayTag Silence = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Silence"));
	inline const FGameplayTag Confused = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Confused"));
	inline const FGameplayTag Groggy = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Groggy"));
	inline const FGameplayTag Parried = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Parried"));

	// StatusEffectTags
	inline const FGameplayTag Running = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Stat.Running"));
	inline const FGameplayTag Slow = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Stat.Slow"));
	inline const FGameplayTag AttackDown = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Stat.AttackDown"));
	inline const FGameplayTag Burn = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.DoT.Burn"));
	inline const FGameplayTag Poison = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.DoT.Poison"));

	// AIEffectTags
	inline const FGameplayTag Idle = FGameplayTag::RequestGameplayTag(TEXT("Effect.AIState.Idle"));
	inline const FGameplayTag Suspicious = FGameplayTag::RequestGameplayTag(TEXT("Effect.AIState.Suspicious"));
	inline const FGameplayTag Alert = FGameplayTag::RequestGameplayTag(TEXT("Effect.AIState.Alert"));
	inline const FGameplayTag Combat = FGameplayTag::RequestGameplayTag(TEXT("Effect.AIState.Combat"));
	inline const FGameplayTag Patrol = FGameplayTag::RequestGameplayTag(TEXT("Effect.AIState.Patrol"));
	inline const FGameplayTag Return = FGameplayTag::RequestGameplayTag(TEXT("Effect.AIState.Return"));
}

namespace StatTags
{
	// BaseStatTags
	inline const FGameplayTag LIFE = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.LIFE"));
	inline const FGameplayTag STR = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.STR"));
	inline const FGameplayTag DEX = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.DEX"));
	inline const FGameplayTag END = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.END"));
	inline const FGameplayTag SKL = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.SKL"));

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
	inline const FGameplayTag WeaponSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.WeaponSkill"));
	inline const FGameplayTag MagicSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill"));
	inline const FGameplayTag Dash = FGameplayTag::RequestGameplayTag(TEXT("Skill.Dash"));
	inline const FGameplayTag Parry = FGameplayTag::RequestGameplayTag(TEXT("Skill.Parry"));

	inline const FGameplayTag SuperAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.SuperAttack"));
	inline const FGameplayTag SpecialAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.SpecialAttack"));
	inline const FGameplayTag EvadeAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.EvadeAttack"));
	inline const FGameplayTag JumpAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.JumpAttack"));
	inline const FGameplayTag RangedAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.RangedAttack"));

	inline const FGameplayTag LevelStart = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.LevelStart"));
}

namespace SkillTags
{
	inline const FGameplayTag BaseAttackSword1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.first.Sword"));
	inline const FGameplayTag BaseAttackSword2 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.second.Sword"));
	inline const FGameplayTag BaseAttackSword3 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.third.Sword"));

	inline const FGameplayTag WeaponSkillSword1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.WeaponSkill.first.Sword"));

	inline const FGameplayTag BaseAttackSpear1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.first.Spear"));
	inline const FGameplayTag PlayerDash = FGameplayTag::RequestGameplayTag(TEXT("Skill.Dash.Player"));
	inline const FGameplayTag PlayerParry = FGameplayTag::RequestGameplayTag(TEXT("Skill.Parry.Player"));

	inline const FGameplayTag LaserSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.Laser"));
	inline const FGameplayTag CurvedProjectile = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.CurvedProjectile"));

	// Boss1
	inline const FGameplayTag Boss1BaseAttack1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.BaseAttack.first"));
	inline const FGameplayTag Boss1BaseAttack2 = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.BaseAttack.second"));
	inline const FGameplayTag Boss1BaseAttack3 = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.BaseAttack.third"));

	inline const FGameplayTag Boss1SuperAttack1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.SuperAttack.first"));
	inline const FGameplayTag Boss1SuperAttack2 = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.SuperAttack.second"));

	inline const FGameplayTag Boss1SpecialAttack1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.SpecialAttack.first"));
	inline const FGameplayTag Boss1SpecialAttack2 = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.SpecialAttack.second"));

	inline const FGameplayTag Boss1EvadeAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.EvadeAttack"));

	inline const FGameplayTag Boss1JumpAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.JumpAttack"));

	inline const FGameplayTag Boss1RangedAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.RangedAttack"));

	inline const FGameplayTag Boss1LevelStart = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.LevelStart"));
	inline const FGameplayTag RailGunSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.RailGun"));
	inline const FGameplayTag FireBallSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.FireBall"));
	inline const FGameplayTag ZoneyaSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.Zoneya"));
	inline const FGameplayTag InfiniteStaminaSkill= FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.InfiniteStamina"));
}

namespace PawnTags
{
	inline const FGameplayTag Player = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Player"));
	inline const FGameplayTag ProtoNormal = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.Proto"));
	inline const FGameplayTag ProtoElite = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Elite.Proto"));
	inline const FGameplayTag ProtoBoss = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Boss.Proto"));
	inline const FGameplayTag ArrowProto = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.ArrowProto"));
	inline const FGameplayTag AxeSkeleton = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.AxeSkeleton"));
	inline const FGameplayTag Boss1 = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Boss.1"));
	inline const FGameplayTag Boss2 = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Boss.2"));
	inline const FGameplayTag Boss3 = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Boss.3"));
}

namespace ItemTags
{
	inline const FGameplayTag ItemBase = FGameplayTag::RequestGameplayTag(TEXT("Item.Base"));

	//Equippable Item Tags
	inline const FGameplayTag EquippableItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable"));
	inline const FGameplayTag WeaponItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Weapon"));
	inline const FGameplayTag BasicWeapon = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Weapon.Basic"));
	inline const FGameplayTag SwordWeapon = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Weapon.Sword"));
	inline const FGameplayTag AxeWeapon = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Weapon.Axe"));
	inline const FGameplayTag SkillItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Skill"));
	inline const FGameplayTag AccessaryItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Accessary"));

	//Consumable Item Tags
	inline const FGameplayTag ConsumableItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable"));
	inline const FGameplayTag Potion = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.Potion"));
	inline const FGameplayTag Elixir = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.Elixir"));

	//Other Item Tags
	inline const FGameplayTag OtherItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Other"));
	inline const FGameplayTag Key = FGameplayTag::RequestGameplayTag(TEXT("Item.Other.Key"));
}