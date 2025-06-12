#pragma once

#include "GameplayTagContainer.h"

namespace EffectTags
{
	// ControlEffectTags
	inline const FGameplayTag UsingSkill = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill"));
	inline const FGameplayTag ConsumingItem = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.ConsumingItem"));
	inline const FGameplayTag Death = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Death"));
	inline const FGameplayTag Dash = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Dash"));
	inline const FGameplayTag UsingDash = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill.Dash"));
	inline const FGameplayTag UsingParry = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill.Parry"));
	inline const FGameplayTag UsingZoneya = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill.Zoneya"));
	inline const FGameplayTag UsingTeleport = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.UsingSkill.Teleport"));
	inline const FGameplayTag LockOn = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.LockOn"));
	inline const FGameplayTag Groggy = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Groggy"));
	inline const FGameplayTag Parried = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Parried"));

	inline const FGameplayTag ControlDebuff = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Debuff"));
	inline const FGameplayTag ControlBuff = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Buff"));

	inline const FGameplayTag Stiffness = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Debuff.Stiffness"));
	inline const FGameplayTag Stun = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Debuff.Stun"));
	inline const FGameplayTag Silence = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Debuff.Silence"));
	inline const FGameplayTag Confused = FGameplayTag::RequestGameplayTag(TEXT("Effect.Control.Debuff.Confused"));
	
	// StatusEffectTags
	inline const FGameplayTag Running = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Running"));

	inline const FGameplayTag StatusDebuff = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Debuff"));
	inline const FGameplayTag StatusBuff = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Buff"));
	
	inline const FGameplayTag Slow = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Debuff.Slow"));
	inline const FGameplayTag AttackDown = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Debuff.AttackDown"));
	inline const FGameplayTag Burn = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Debuff.Burn"));
	inline const FGameplayTag Poison = FGameplayTag::RequestGameplayTag(TEXT("Effect.Status.Debuff.Poison"));

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
	inline const FGameplayTag BaseStat = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base"));
	inline const FGameplayTag Level = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.Level"));
	inline const FGameplayTag LIFE = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.LIFE"));
	inline const FGameplayTag STR = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.STR"));
	inline const FGameplayTag SPL = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.SPL"));
	inline const FGameplayTag END = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.END"));
	inline const FGameplayTag BaseAttackPower = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.BaseAttackPower"));
	inline const FGameplayTag BaseMagicPower = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.BaseMagicPower"));
	inline const FGameplayTag BaseMaxHealth = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.BaseMaxHealth"));
	inline const FGameplayTag BaseMaxStamina = FGameplayTag::RequestGameplayTag(TEXT("Stat.Base.BaseMaxStamina"));

	// Base -> Battle Coefficients
	inline const FGameplayTag StatCoefficient = FGameplayTag::RequestGameplayTag(TEXT("Stat.Coefficient"));
	inline const FGameplayTag AttackPowerCoefficient = FGameplayTag::RequestGameplayTag(TEXT("Stat.Coefficient.AttackPower"));
	inline const FGameplayTag MagicPowerCoefficient = FGameplayTag::RequestGameplayTag(TEXT("Stat.Coefficient.MagicPower"));
	inline const FGameplayTag MaxHealthCoefficient = FGameplayTag::RequestGameplayTag(TEXT("Stat.Coefficient.MaxHealth"));
	inline const FGameplayTag MaxStaminaCoefficient = FGameplayTag::RequestGameplayTag(TEXT("Stat.Coefficient.MaxStamina"));
	inline const FGameplayTag LevelUpCoefficientB = FGameplayTag::RequestGameplayTag(TEXT("Stat.Coefficient.LevelUpB"));
	inline const FGameplayTag LevelUpCoefficientC = FGameplayTag::RequestGameplayTag(TEXT("Stat.Coefficient.LevelUpC"));
	inline const FGameplayTag LevelUpCoefficientD = FGameplayTag::RequestGameplayTag(TEXT("Stat.Coefficient.LevelUpD"));
	
	// BattleStatTags
	inline const FGameplayTag BattleStat = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle"));
	inline const FGameplayTag MaxHealth = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.MaxHealth"));
	inline const FGameplayTag MaxStamina = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.MaxStamina"));
	inline const FGameplayTag AttackPower = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.AttackPower"));
	
	inline const FGameplayTag SubAttackPower = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.SubAttackPower"));
	inline const FGameplayTag MagicPower = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.MagicPower"));
	inline const FGameplayTag StaminaRecoveryRate = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.StaminaRecoveryRate"));
	inline const FGameplayTag SpeedCoefficient = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.SpeedCoefficient"));

	inline const FGameplayTag Defense = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.Defense"));
	inline const FGameplayTag MaxShield = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.MaxShield"));
	inline const FGameplayTag MaxGroggyGauge = FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle.MaxGroggyGauge"));
	
	// VariableStatTags
	inline const FGameplayTag VariableStat = FGameplayTag::RequestGameplayTag(TEXT("Stat.Variable"));
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

	// Boss1
	inline const FGameplayTag SuperAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.SuperAttack"));
	inline const FGameplayTag SpecialAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.SpecialAttack"));
	inline const FGameplayTag EvadeAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.EvadeAttack"));
	inline const FGameplayTag JumpAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.JumpAttack"));
	inline const FGameplayTag RangedAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.RangedAttack"));

	// Boss2 Skill Group Tag
	inline const FGameplayTag Boss2BaseAttack1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.BaseAttack1"));
	inline const FGameplayTag Boss2BaseAttack2 = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.BaseAttack2"));
	inline const FGameplayTag Boss2Circling = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.Circling"));

	// Boss4
	inline const FGameplayTag Boss4FlameStrike = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.FlameStrike"));
	inline const FGameplayTag Boss4SpawnClone = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.SpawnClone"));
	inline const FGameplayTag Boss4InfernoSpike = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.InfernoSpike"));
	inline const FGameplayTag Boss4HellfireFall = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.HellfireFall"));
	inline const FGameplayTag Boss4BlazeSphere = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.BlazeSphere"));
	inline const FGameplayTag Boss4BackDash = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.BackDash"));
	inline const FGameplayTag Boss4Teleport = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.Teleport"));
}

namespace SkillTags
{
	// Player
	inline const FGameplayTag BaseAttackSword1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.first.Sword"));
	inline const FGameplayTag BaseAttackSword2 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.second.Sword"));
	inline const FGameplayTag BaseAttackSword3 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.third.Sword"));
	inline const FGameplayTag BaseAttackAxe1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.first.Axe"));
	inline const FGameplayTag BaseAttackAxe2 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.second.Axe"));
	inline const FGameplayTag BaseAttackAxe3 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.third.Axe"));
	inline const FGameplayTag BaseAttackClaw1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.first.Claw"));
	inline const FGameplayTag BaseAttackClaw2 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.second.Claw"));
	inline const FGameplayTag BaseAttackClaw3 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.third.Claw"));
	inline const FGameplayTag BaseAttackClaw4 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.fourth.Claw"));

	inline const FGameplayTag WeaponSkillSword1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.WeaponSkill.first.Sword"));
	inline const FGameplayTag WeaponSkillAxe1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.WeaponSkill.first.Axe"));
	inline const FGameplayTag WeaponSkillClaw1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.WeaponSkill.first.Claw"));

	inline const FGameplayTag BaseAttackSpear1 = FGameplayTag::RequestGameplayTag(TEXT("Skill.BaseAttack.first.Spear"));
	inline const FGameplayTag PlayerDash = FGameplayTag::RequestGameplayTag(TEXT("Skill.Dash.Player"));
	inline const FGameplayTag PlayerParry = FGameplayTag::RequestGameplayTag(TEXT("Skill.Parry.Player"));

	inline const FGameplayTag MagicLaserSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.Laser"));
	inline const FGameplayTag MagicRailGunSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.RailGun"));
	inline const FGameplayTag MagicFireBallSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.FireBall"));
	inline const FGameplayTag MagicZoneyaSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.Zoneya"));
	inline const FGameplayTag MagicInfiniteStaminaSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.InfiniteStamina"));
	inline const FGameplayTag MagicTeleportSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.Teleport"));
	inline const FGameplayTag MagicItemScanningSkill = FGameplayTag::RequestGameplayTag(TEXT("Skill.MagicSkill.ItemScanning"));	

	// NomalMonster
	inline const FGameplayTag AxeSkeletonComboAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.NomalMonster.AxeSkeleton.ComboAttack"));
	inline const FGameplayTag SwordSkeletonBaseAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.NomalMonster.SwordSkeleton.BaseAttack"));
	inline const FGameplayTag DualBladeSkeletonBaseAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.NomalMonster.DualBladeSkeleton.BaseAttack"));
	inline const FGameplayTag DualBladeSkeletonJumpAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.NomalMonster.DualBladeSkeleton.JumpAttack"));
	inline const FGameplayTag KnifeThrow = FGameplayTag::RequestGameplayTag(TEXT("Skill.NomalMonster.KnifeSkeleton.KnifeThrow"));
	inline const FGameplayTag WoodSwiping = FGameplayTag::RequestGameplayTag(TEXT("Skill.NomalMonster.Wood.Swiping"));
	inline const FGameplayTag WoodHurricaneKick = FGameplayTag::RequestGameplayTag(TEXT("Skill.NomalMonster.Wood.HurricaneKick"));
	inline const FGameplayTag MinionBaseAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.NomalMonster.Minion.BaseAttack"));
	inline const FGameplayTag GunMinionFire = FGameplayTag::RequestGameplayTag(TEXT("Skill.NomalMonster.GunMinion.MinionFire"));
	inline const FGameplayTag GolemSmash = FGameplayTag::RequestGameplayTag(TEXT("Skill.NomalMonster.EliteGolem.GolemSmash"));


	// Boss1
	inline const FGameplayTag Boss1LevelStart = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.LevelStart"));
	inline const FGameplayTag Boss1Parried = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss1.Parried"));

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

	// Boss2
	inline const FGameplayTag Boss2LevelStart = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.LevelStart"));
	inline const FGameplayTag Boss2BaseAttack1First = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.BaseAttack1.first"));
	inline const FGameplayTag Boss2BaseAttack1Second = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.BaseAttack1.second"));
	inline const FGameplayTag Boss2BaseAttack1Third = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.BaseAttack1.third"));
	inline const FGameplayTag Boss2BaseAttack1Fourth = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.BaseAttack1.Fourth"));

	inline const FGameplayTag Boss2BaseAttack2First = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.BaseAttack2.first"));
	inline const FGameplayTag Boss2BaseAttack2Second = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.BaseAttack2.second"));
	inline const FGameplayTag Boss2BaseAttack2Third = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.BaseAttack2.third"));
	inline const FGameplayTag Boss2BaseAttack2Fourth = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.BaseAttack2.fourth"));
	inline const FGameplayTag Boss2BaseAttack2Fifth = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.BaseAttack2.fifth"));

	inline const FGameplayTag Boss2Circling = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.Circling"));

	inline const FGameplayTag Boss2DashAttackFirst = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.DashAttack.first"));
	inline const FGameplayTag Boss2DashAttackSecond = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.DashAttack.second"));
	inline const FGameplayTag Boss2DashAttackThird = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.DashAttack.third"));
	inline const FGameplayTag Boss2DashAttackBFirst = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.DashAttackB.first"));
	inline const FGameplayTag Boss2DashAttackBSecond = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.DashAttackB.second"));
	inline const FGameplayTag Boss2EvadeAttackFirst = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.EvadeAttack.first"));
	inline const FGameplayTag Boss2EvadeAttackSecond = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.EvadeAttack.second"));
	inline const FGameplayTag Boss2Dash = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss2.Dash"));

	// Boss3
	inline const FGameplayTag Boss3LevelStart = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss3.LevelStart"));
	inline const FGameplayTag Boss3BaseAttack1first = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss3.BaseAttack1.first"));
	inline const FGameplayTag Boss3BaseAttack1second = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss3.BaseAttack1.second"));
	inline const FGameplayTag Boss3BaseAttack1third = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss3.BaseAttack1.third"));
	inline const FGameplayTag Boss3BaseAttack2first = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss3.BaseAttack2.first"));
	inline const FGameplayTag Boss3BaseAttack2second = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss3.BaseAttack2.second"));
	inline const FGameplayTag Boss3RockThrow = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss3.RockThrow.RockThrow"));
	inline const FGameplayTag Boss3TurningAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss3.TurningAttack.TurningAttack"));
	inline const FGameplayTag Boss3GroundFall = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss3.GroundFall.GroundFall"));
	inline const FGameplayTag Boss3Grab = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss3.Grab.Grab"));
	inline const FGameplayTag Boss3GrabAttack = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss3.Grab.GrabAttack"));

	// Boss4
	inline const FGameplayTag Boss4BaseAttackFirst = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.BaseAttack.first"));
	inline const FGameplayTag Boss4BaseAttackSecond = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.BaseAttack.second"));
	inline const FGameplayTag Boss4FlameStrike = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.FlameStrike"));
	inline const FGameplayTag Boss4SpawnClone = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.SpawnClone"));
	inline const FGameplayTag Boss4InfernoSpike = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.InfernoSpike"));
	inline const FGameplayTag Boss4HellfireFall = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.HellfireFall"));
	inline const FGameplayTag Boss4BlazeSphere = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.BlazeSphere"));
	inline const FGameplayTag Boss4BackDash = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.BackDash"));
	inline const FGameplayTag Boss4Teleport = FGameplayTag::RequestGameplayTag(TEXT("Skill.Boss4.Teleport"));
}

namespace PawnTags
{
	inline const FGameplayTag Player = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Player"));
	inline const FGameplayTag ProtoNormal = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.Proto"));
	inline const FGameplayTag ProtoElite = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Elite.Proto"));
	inline const FGameplayTag ProtoBoss = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Boss.Proto"));
	inline const FGameplayTag ArrowProto = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.ArrowProto"));
	inline const FGameplayTag AxeSkeleton = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.AxeSkeleton"));
	inline const FGameplayTag SwordSkeleton = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.SwordSkeleton"));
	inline const FGameplayTag DualBladeSkeleton = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.DualBladeSkeleton"));
	inline const FGameplayTag KnifeSkeleton = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.KnifeSkeleton"));
	inline const FGameplayTag Wood = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.Wood"));
	inline const FGameplayTag Minion = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.Minion"));
	inline const FGameplayTag GunMinion = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.GunMinion"));
	inline const FGameplayTag EliteGolem = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Normal.EliteGolem"));
	inline const FGameplayTag Boss1 = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Boss.1"));
	inline const FGameplayTag Boss2 = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Boss.2"));
	inline const FGameplayTag Boss3 = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Boss.3"));
	inline const FGameplayTag Boss4 = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Boss.4"));
	inline const FGameplayTag Boss4Clone = FGameplayTag::RequestGameplayTag(TEXT("Pawn.Enemy.Boss.4.Clone"));
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
	inline const FGameplayTag ClawWeapon = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Weapon.Claw"));
	inline const FGameplayTag HammerWeapon = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Weapon.Hammer"));
	inline const FGameplayTag SkillItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Skill"));
	inline const FGameplayTag RailGunSkillItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Skill.RailGun"));
	inline const FGameplayTag FireBallSkillItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Skill.FireBall"));
	inline const FGameplayTag TeleportSkillItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Skill.Teleport"));
	inline const FGameplayTag ZoneyaSkillItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Skill.Zoneya"));
	inline const FGameplayTag InfiniteStaminaSkillItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Skill.InfiniteStamina"));
	inline const FGameplayTag AccessoryItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Accessory"));
	inline const FGameplayTag RingOfHealthAccessoryItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Accessory.RingOfHealth"));
	inline const FGameplayTag NecklaceOfStaminaAccessoryItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Accessory.NecklaceOfStamina"));
	inline const FGameplayTag EaringOfAttackPowerAccessoryItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Accessory.EaringOfAttackPower"));
	inline const FGameplayTag SlowImmuneAccessoryItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Accessory.SlowImmuneAc"));
	inline const FGameplayTag BurnImmuneAccessoryItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Accessory.BurnImmuneAc"));
	inline const FGameplayTag PoisonImmuneAccessoryItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Accessory.PoisonImmuneAc"));
	inline const FGameplayTag AttackDownImmuneAccessoryItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Equippable.Accessory.AttackDownImmuneAc"));

	//Consumable Item Tags
	inline const FGameplayTag ConsumableItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable"));
	inline const FGameplayTag Potion = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.Potion"));
	inline const FGameplayTag PotionBoosted = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.Potion.Boosted"));
	inline const FGameplayTag AddMaxPotion = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.AddMaxPotion"));
	inline const FGameplayTag StaminaPotion = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.StaminaPotion"));
	inline const FGameplayTag Elixir = FGameplayTag::RequestGameplayTag(TEXT("Item.Consumable.Elixir"));

	//Other Item Tags
	inline const FGameplayTag OtherItem = FGameplayTag::RequestGameplayTag(TEXT("Item.Other"));
	inline const FGameplayTag Key = FGameplayTag::RequestGameplayTag(TEXT("Item.Other.Key"));
	inline const FGameplayTag EnergyCore = FGameplayTag::RequestGameplayTag(TEXT("Item.Other.EnergyCore"));
}

namespace TraceTags
{
	// Axe
	inline const FGameplayTag HandRightAxe = FGameplayTag::RequestGameplayTag(TEXT("Trace.Hand.Right.Axe"));

	// Claw
	inline const FGameplayTag HandLeftClaw = FGameplayTag::RequestGameplayTag(TEXT("Trace.Hand.Left.Claw"));
	inline const FGameplayTag HandRightClaw = FGameplayTag::RequestGameplayTag(TEXT("Trace.Hand.Right.Claw"));
	
	// Sword
	inline const FGameplayTag HandRightSword = FGameplayTag::RequestGameplayTag(TEXT("Trace.Hand.Right.Sword"));
}

namespace WorldActorTags
{
	inline const FGameplayTag ItemDropped = FGameplayTag::RequestGameplayTag(TEXT("WorldActor.Item.ItemDropped"));
	inline const FGameplayTag OpenableChestItem = FGameplayTag::RequestGameplayTag(TEXT("WorldActor.Item.OpenableChestItem"));
	inline const FGameplayTag ItemContainer = FGameplayTag::RequestGameplayTag(TEXT("WorldActor.Item.ItemContainer"));
}