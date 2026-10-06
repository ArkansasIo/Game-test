#pragma bank 4

/**
 * Implementations for the Necromancer, Rune Paladin, Shadow Assassin and
 * Stormcaller classes.
 *
 * Each class follows the same shape as the original four (see `player.c`):
 * a `_base_attack` used by the Fight command, six abilities, an
 * `_update_stats` that assigns the class's power tiers, and a
 * `get_*_grant_message` that names an ability when it is learned.
 */
#include <stdio.h>

#include "battle.effects.h"
#include "encounter.h"
#include "player.h"
#include "stats.h"
#include "strings.h"

//------------------------------------------------------------------------------
// Necromancer - a dark caster that drains life and fields the dead.
//------------------------------------------------------------------------------

void necromancer_update_stats(void)
{
  update_stats_public(
      C_TIER, // hp
      S_TIER, // sp
      C_TIER, // atk
      C_TIER, // def
      S_TIER, // matk
      A_TIER, // mdef
      B_TIER  // agl
  );
}

void necromancer_base_attack(void)
{
  sprintf(battle_pre_message, str_player_necro_attack);

  Monster *target = encounter.target;
  if (!roll_attack_player(player.matk, target->mdef))
  {
    PLAYER_MISS;
    return;
  }

  const uint16_t base_dmg = get_player_damage(player.level, C_TIER);
  damage_monster_public(base_dmg, DAMAGE_DARK);

  // Draining touch returns a portion of the damage as health.
  heal_player(base_dmg >> 2);
}

void necromancer_drain_life(void)
{
  sprintf(battle_pre_message, str_player_necro_drain_life);

  Monster *target = encounter.target;
  if (!roll_attack_player(player.matk, target->mdef))
  {
    PLAYER_MISS;
    return;
  }

  const uint16_t base_dmg = get_player_damage(
      level_offset(player.level, 5), B_TIER);
  damage_monster_public(base_dmg, DAMAGE_DARK);
  heal_player(base_dmg >> 1);
}

void necromancer_bone_armor(void)
{
  sprintf(battle_pre_message, str_player_necro_bone_armor);
  SKIP_POST_MSG;
  apply_def_up(encounter.player_status_effects, A_TIER, 0);
}

void necromancer_raise_dead(void)
{
  sprintf(battle_pre_message, str_player_necro_raise_dead);
  SKIP_POST_MSG;
  apply_regen(encounter.player_status_effects, A_TIER, 0);
}

void necromancer_curse(void) {
  sprintf(battle_pre_message, str_player_necro_curse);
  SKIP_POST_MSG;

  Monster *target = encounter.target;
  apply_atk_down(target->status_effects, A_TIER, 0, target->debuff_immune);
  apply_def_down(target->status_effects, A_TIER, 0, target->debuff_immune);
}

void necromancer_soul_harvest(void)
{
  sprintf(battle_pre_message, str_player_necro_soul_harvest);

  const uint8_t level = level_offset(player.level, 6);
  const uint16_t base_damage = get_player_damage(level, A_TIER);
  uint8_t hits = damage_all_public(base_damage, player.matk, true, DAMAGE_DARK);

  if (hits == 0)
    PLAYER_MISS_ALL;
  else
  {
    SKIP_POST_MSG;
    heal_player(base_damage >> 1);
  }
}

void necromancer_army_of_the_dead(void)
{
  sprintf(battle_pre_message, str_player_necro_army_dead);

  PowerTier tier = player.level > 75 ? S_TIER : A_TIER;
  const uint8_t level = level_offset(player.level, 8);
  const uint16_t base_damage = get_player_damage(level, tier);
  uint8_t hits = damage_all_public(base_damage, player.matk, true, DAMAGE_DARK);

  if (hits == 0)
    PLAYER_MISS_ALL;
  else
    SKIP_POST_MSG;
}

const char *get_necromancer_grant_message(AbilityFlag flag)
{
  switch (flag)
  {
  case ABILITY_1:
    return str_gain_ability_necromancer1;
  case ABILITY_2:
    return str_gain_ability_necromancer2;
  case ABILITY_3:
    return str_gain_ability_necromancer3;
  case ABILITY_4:
    return str_gain_ability_necromancer4;
  default:
    return str_gain_ability_necromancer5;
  }
}

//------------------------------------------------------------------------------
// Rune Paladin - a holy tank that heals and shields.
//------------------------------------------------------------------------------

void rune_paladin_update_stats(void)
{
  update_stats_public(
      A_TIER, // hp
      B_TIER, // sp
      A_TIER, // atk
      S_TIER, // def
      B_TIER, // matk
      A_TIER, // mdef
      C_TIER  // agl
  );
}

void rune_paladin_base_attack(void)
{
  sprintf(battle_pre_message, str_player_pally_attack);

  Monster *target = encounter.target;
  if (!roll_attack_player(player.atk, target->def))
  {
    PLAYER_MISS;
    return;
  }

  const uint16_t base_dmg = get_player_damage(player.level, B_TIER);
  damage_monster_public(base_dmg, DAMAGE_PHYSICAL);
}

void rune_paladin_lay_on_hands(void)
{
  sprintf(battle_pre_message, str_player_pally_lay_hands);
  heal_player(player.max_hp / 2);
}

void rune_paladin_shield_of_faith(void)
{
  sprintf(battle_pre_message, str_player_pally_shield_faith);
  SKIP_POST_MSG;
  apply_def_up(encounter.player_status_effects, A_TIER, 0);
}

void rune_paladin_smite(void)
{
  sprintf(battle_pre_message, str_player_pally_smite);

  Monster *target = encounter.target;
  if (!roll_attack_player(player.atk, target->def))
  {
    PLAYER_MISS;
    return;
  }

  PowerTier tier = player.level > 60 ? S_TIER : A_TIER;
  const uint16_t base_dmg = get_player_damage(
      level_offset(player.level, 6), tier);
  damage_monster_public(base_dmg, DAMAGE_LIGHT);
}

void rune_paladin_consecrate(void)
{
  sprintf(battle_pre_message, str_player_pally_consecrate);

  PowerTier tier = player.level > 60 ? S_TIER : B_TIER;
  const uint8_t level = level_offset(player.level, 4);
  const uint16_t base_damage = get_player_damage(level, tier);
  uint8_t hits = damage_all_public(base_damage, player.matk, true, DAMAGE_LIGHT);

  if (hits == 0)
    PLAYER_MISS_ALL;
  else
    SKIP_POST_MSG;
}

void rune_paladin_divine_shield(void)
{
  sprintf(battle_pre_message, str_player_pally_divine_shield);
  SKIP_POST_MSG;
  apply_def_up(encounter.player_status_effects, S_TIER, 0);
  apply_special(SPECIAL_EVASION);
}

void rune_paladin_hammer_of_god(void)
{
  sprintf(battle_pre_message, str_player_pally_hammer_god);

  Monster *target = encounter.target;
  if (!roll_attack_player(player.atk, target->def))
  {
    PLAYER_MISS;
    return;
  }

  const uint16_t base_dmg = get_player_damage(
      level_offset(player.level, 12), S_TIER);
  damage_monster_public(base_dmg, DAMAGE_LIGHT);
}

const char *get_rune_paladin_grant_message(AbilityFlag flag)
{
  switch (flag)
  {
  case ABILITY_1:
    return str_gain_ability_rune_paladin1;
  case ABILITY_2:
    return str_gain_ability_rune_paladin2;
  case ABILITY_3:
    return str_gain_ability_rune_paladin3;
  case ABILITY_4:
    return str_gain_ability_rune_paladin4;
  default:
    return str_gain_ability_rune_paladin5;
  }
}

//------------------------------------------------------------------------------
// Shadow Assassin - a burst damage dealer built on critical hits.
//------------------------------------------------------------------------------

void shadow_assassin_update_stats(void)
{
  update_stats_public(
      C_TIER, // hp
      C_TIER, // sp
      S_TIER, // atk
      C_TIER, // def
      C_TIER, // matk
      C_TIER, // mdef
      S_TIER  // agl
  );
}

void shadow_assassin_base_attack(void)
{
  sprintf(battle_pre_message, str_player_assassin_attack);

  Monster *target = encounter.target;
  if (!roll_attack_player(player.atk, target->def))
  {
    PLAYER_MISS;
    return;
  }

  const uint16_t base_dmg = get_player_damage(player.level, B_TIER);
  damage_monster_public(base_dmg, DAMAGE_PHYSICAL);
}

void shadow_assassin_shadow_step(void)
{
  sprintf(battle_pre_message, str_player_assassin_shadow_step);
  SKIP_POST_MSG;
  apply_haste(encounter.player_status_effects, A_TIER, 0);
  apply_special(SPECIAL_EVASION);
}

void shadow_assassin_poison_blade(void)
{
  sprintf(battle_pre_message, str_player_assassin_poison_blade);

  Monster *target = encounter.target;
  if (!roll_attack_player(player.atk, target->def))
  {
    PLAYER_MISS;
    return;
  }

  const uint16_t base_dmg = get_player_damage(
      level_offset(player.level, 4), B_TIER);
  damage_monster_public(base_dmg, DAMAGE_PHYSICAL);
  apply_poison(target->status_effects, A_TIER, 0, target->debuff_immune);
}

void shadow_assassin_assassinate(void)
{
  sprintf(battle_pre_message, str_player_assassin_assassinate);

  Monster *target = encounter.target;
  if (!roll_attack_player(player.atk, target->def))
  {
    PLAYER_MISS;
    return;
  }

  const uint16_t base_dmg = get_player_damage(
      level_offset(player.level, 10), S_TIER);
  damage_monster_public(base_dmg, DAMAGE_PHYSICAL);
}

void shadow_assassin_vanish(void)
{
  sprintf(battle_pre_message, str_player_assassin_vanish);
  SKIP_POST_MSG;
  apply_special(SPECIAL_EVASION);
  apply_haste(encounter.player_status_effects, B_TIER, 0);
}

void shadow_assassin_death_mark(void)
{
  sprintf(battle_pre_message, str_player_assassin_death_mark);
  SKIP_POST_MSG;

  Monster *target = encounter.target;
  apply_def_down(target->status_effects, A_TIER, 0, target->debuff_immune);
}

void shadow_assassin_thousand_cuts(void)
{
  sprintf(battle_pre_message, str_player_assassin_thousand_cuts);

  PowerTier tier = player.level > 70 ? S_TIER : A_TIER;
  const uint8_t level = level_offset(player.level, 5);
  const uint16_t base_damage = get_player_damage(level, tier);
  uint8_t hits = damage_all_public(base_damage, player.atk, false, DAMAGE_PHYSICAL);

  if (hits == 0)
    PLAYER_MISS_ALL;
  else
    SKIP_POST_MSG;
}

const char *get_shadow_assassin_grant_message(AbilityFlag flag)
{
  switch (flag)
  {
  case ABILITY_1:
    return str_gain_ability_shadow_assassin1;
  case ABILITY_2:
    return str_gain_ability_shadow_assassin2;
  case ABILITY_3:
    return str_gain_ability_shadow_assassin3;
  case ABILITY_4:
    return str_gain_ability_shadow_assassin4;
  default:
    return str_gain_ability_shadow_assassin5;
  }
}

//------------------------------------------------------------------------------
// Stormcaller - an air caster with high damage and crowd control.
//------------------------------------------------------------------------------

void stormcaller_update_stats(void)
{
  update_stats_public(
      C_TIER, // hp
      A_TIER, // sp
      C_TIER, // atk
      C_TIER, // def
      S_TIER, // matk
      B_TIER, // mdef
      A_TIER  // agl
  );
}

void stormcaller_base_attack(void)
{
  sprintf(battle_pre_message, str_player_storm_attack);

  Monster *target = encounter.target;
  if (!roll_attack_player(player.matk, target->mdef))
  {
    PLAYER_MISS;
    return;
  }

  const uint16_t base_dmg = get_player_damage(player.level, C_TIER);
  damage_monster_public(base_dmg, DAMAGE_AIR);
}

void stormcaller_static_charge(void)
{
  sprintf(battle_pre_message, str_player_storm_static_charge);
  SKIP_POST_MSG;
  apply_atk_up(encounter.player_status_effects, A_TIER, 0);
}

void stormcaller_chain_lightning(void)
{
  sprintf(battle_pre_message, str_player_storm_chain_light);

  const uint8_t level = level_offset(player.level, 4);
  const uint16_t base_damage = get_player_damage(level, B_TIER);
  uint8_t hits = damage_all_public(base_damage, player.matk, true, DAMAGE_AIR);

  if (hits == 0)
    PLAYER_MISS_ALL;
  else
    SKIP_POST_MSG;
}

void stormcaller_storm_shield(void)
{
  sprintf(battle_pre_message, str_player_storm_storm_shield);
  SKIP_POST_MSG;
  apply_def_up(encounter.player_status_effects, A_TIER, 0);
  apply_regen(encounter.player_status_effects, B_TIER, 0);
}

void stormcaller_lightning_storm(void)
{
  sprintf(battle_pre_message, str_player_storm_light_storm);

  PowerTier tier = player.level > 70 ? S_TIER : A_TIER;
  const uint8_t level = level_offset(player.level, 7);
  const uint16_t base_damage = get_player_damage(level, tier);
  uint8_t hits = damage_all_public(base_damage, player.matk, true, DAMAGE_AIR);

  if (hits == 0)
    PLAYER_MISS_ALL;
  else
    SKIP_POST_MSG;
}

void stormcaller_thunder_god(void)
{
  sprintf(battle_pre_message, str_player_storm_thunder_god);

  Monster *target = encounter.target;
  if (!roll_attack_player(player.matk, target->mdef))
  {
    PLAYER_MISS;
    return;
  }

  const uint16_t base_dmg = get_player_damage(
      level_offset(player.level, 14), S_TIER);
  damage_monster_public(base_dmg, DAMAGE_AIR);
}

const char *get_stormcaller_grant_message(AbilityFlag flag)
{
  switch (flag)
  {
  case ABILITY_1:
    return str_gain_ability_stormcaller1;
  case ABILITY_2:
    return str_gain_ability_stormcaller2;
  case ABILITY_3:
    return str_gain_ability_stormcaller3;
  case ABILITY_4:
    return str_gain_ability_stormcaller4;
  default:
    return str_gain_ability_stormcaller5;
  }
}

//------------------------------------------------------------------------------
// Ability data
//
// These live on bank 4 rather than alongside the original four classes in
// `player.data.c`, which is on bank 0. Bank 0 is the fixed region and was
// already at 100% capacity, so new data has to be banked.
//------------------------------------------------------------------------------

const Ability necromancer0 = {
  1, str_ability_necro_drain_life,
  TARGET_SINGLE, 5, necromancer_drain_life,
};

const Ability necromancer1 = {
  2, str_ability_necro_bone_armor,
  TARGET_SELF, 9, necromancer_bone_armor,
};

const Ability necromancer2 = {
  3, str_ability_necro_raise_dead,
  TARGET_SELF, 16, necromancer_raise_dead,
};

const Ability necromancer3 = {
  4, str_ability_necro_curse,
  TARGET_SINGLE, 22, necromancer_curse,
};

const Ability necromancer4 = {
  5, str_ability_necro_soul_harvest,
  TARGET_ALL, 30, necromancer_soul_harvest,
};

const Ability necromancer5 = {
  6, str_ability_necro_army_dead,
  TARGET_ALL, 38, necromancer_army_of_the_dead,
};

//------------------------------------------------------------------------------

const Ability rune_paladin0 = {
  1, str_ability_pally_lay_hands,
  TARGET_SELF, 6, rune_paladin_lay_on_hands,
};

const Ability rune_paladin1 = {
  2, str_ability_pally_shield_faith,
  TARGET_SELF, 11, rune_paladin_shield_of_faith,
};

const Ability rune_paladin2 = {
  3, str_ability_pally_smite,
  TARGET_SINGLE, 17, rune_paladin_smite,
};

const Ability rune_paladin3 = {
  4, str_ability_pally_consecrate,
  TARGET_ALL, 24, rune_paladin_consecrate,
};

const Ability rune_paladin4 = {
  5, str_ability_pally_divine_shield,
  TARGET_SELF, 31, rune_paladin_divine_shield,
};

const Ability rune_paladin5 = {
  6, str_ability_pally_hammer_god,
  TARGET_SINGLE, 39, rune_paladin_hammer_of_god,
};

//------------------------------------------------------------------------------

const Ability shadow_assassin0 = {
  1, str_ability_assassin_shadow_step,
  TARGET_SELF, 5, shadow_assassin_shadow_step,
};

const Ability shadow_assassin1 = {
  2, str_ability_assassin_poison_blade,
  TARGET_SINGLE, 10, shadow_assassin_poison_blade,
};

const Ability shadow_assassin2 = {
  3, str_ability_assassin_assassinate,
  TARGET_SINGLE, 18, shadow_assassin_assassinate,
};

const Ability shadow_assassin3 = {
  4, str_ability_assassin_vanish,
  TARGET_SELF, 23, shadow_assassin_vanish,
};

const Ability shadow_assassin4 = {
  5, str_ability_assassin_death_mark,
  TARGET_SINGLE, 29, shadow_assassin_death_mark,
};

const Ability shadow_assassin5 = {
  6, str_ability_assassin_thousand,
  TARGET_ALL, 37, shadow_assassin_thousand_cuts,
};

//------------------------------------------------------------------------------

const Ability stormcaller0 = {
  1, str_ability_storm_static_charge,
  TARGET_SELF, 6, stormcaller_static_charge,
};

const Ability stormcaller1 = {
  2, str_ability_storm_chain_light,
  TARGET_ALL, 13, stormcaller_chain_lightning,
};

const Ability stormcaller2 = {
  3, str_ability_storm_storm_shield,
  TARGET_SELF, 19, stormcaller_storm_shield,
};

const Ability stormcaller3 = {
  4, str_ability_storm_light_storm,
  TARGET_ALL, 27, stormcaller_lightning_storm,
};

const Ability stormcaller4 = {
  5, str_ability_storm_thunder_god,
  TARGET_SINGLE, 34, stormcaller_thunder_god,
};

const Ability stormcaller5 = {
  6, str_ability_storm_light_storm,
  TARGET_ALL, 42, stormcaller_lightning_storm,
};

//------------------------------------------------------------------------------
// Test class ability data
//
// Moved here from `player.data.c` (bank 0) to keep the fixed bank from
// overflowing. Bank 0 is shared by the whole game's non-banked code and data.
//------------------------------------------------------------------------------

const Ability test_class0 = {
  1, "Damage All", TARGET_SELF, 0, test_class_ability0
};

const Ability test_class1 = {
  2, "(De)buff", TARGET_SELF, 0, test_class_ability1
};

const Ability test_class2 = {
  3, "SUPERKILL", TARGET_SELF, 0, test_class_ability2
};

const Ability test_class3 = {
  4, "Test 4", TARGET_SELF, 0, test_class_ability3
};

const Ability test_class4 = {
  5, "Test 5", TARGET_SELF, 0, test_class_ability4
};

const Ability test_class5 = {
  6, "Test 6", TARGET_SELF, 0, test_class_ability5
};
