#pragma bank 7
#include "world_zone_encounters.h"
#include "encounter.h"
static const WorldEncounterBand bands[REALM_ZONE_COUNT] = {
  { REALM_ZONE_CROWN_REALM, 1, 8, MONSTER_KOBOLD, MONSTER_GOBLIN, MONSTER_BUGBEAR },
  { REALM_ZONE_GREEN_REALM, 4, 18, MONSTER_GOBLIN, MONSTER_BUGBEAR, MONSTER_OWLBEAR },
  { REALM_ZONE_SUN_REALM, 12, 30, MONSTER_BUGBEAR, MONSTER_GELATINOUS_CUBE, MONSTER_DISPLACER_BEAST },
  { REALM_ZONE_IRON_REALM, 18, 42, MONSTER_OWLBEAR, MONSTER_DISPLACER_BEAST, MONSTER_WILL_O_WISP },
  { REALM_ZONE_MIST_REALM, 28, 55, MONSTER_ZOMBIE, MONSTER_WILL_O_WISP, MONSTER_DEATHKNIGHT },
  { REALM_ZONE_FROST_REALM, 40, 70, MONSTER_WILL_O_WISP, MONSTER_MINDFLAYER, MONSTER_BEHOLDER },
  { REALM_ZONE_SHADOW_REALM, 50, 80, MONSTER_DEATHKNIGHT, MONSTER_MINDFLAYER, MONSTER_BEHOLDER },
  { REALM_ZONE_DRAGON_REALM, 55, 99, MONSTER_BEHOLDER, MONSTER_DEATHKNIGHT, MONSTER_DRAGON },
  { REALM_ZONE_ASTRAL_REALM, 65, 99, MONSTER_MINDFLAYER, MONSTER_BEHOLDER, MONSTER_DRAGON },
  { REALM_ZONE_VOID_REALM, 80, 99, MONSTER_DEATHKNIGHT, MONSTER_BEHOLDER, MONSTER_DRAGON }
};
const WorldEncounterBand *world_encounter_band(RealmZoneId realm) BANKED {
  if (realm >= REALM_ZONE_COUNT) return &bands[REALM_ZONE_CROWN_REALM];
  return &bands[realm];
}
MonsterType world_zone_monster(RealmZoneId realm, uint8_t roll) BANKED {
  const WorldEncounterBand *b = world_encounter_band(realm);
  if (roll < 160) return b->primary;
  if (roll < 240) return b->secondary;
  return b->elite;
}
uint8_t world_zone_level(RealmZoneId realm, uint8_t player_level, uint8_t roll) BANKED {
  const WorldEncounterBand *b = world_encounter_band(realm);
  uint8_t level = player_level;
  if (level < b->min_level) level = b->min_level;
  if (level > b->max_level) level = b->max_level;
  if (roll % 3 == 0 && level > b->min_level) level--;
  else if (roll % 3 == 2 && level < b->max_level) level++;
  return level;
}

bool generate_world_encounter(uint8_t player_level) BANKED {
  const RealmZoneId realm = realm_zone_at(world_chunk_x, world_chunk_y);
  const WorldEncounterBand *b = world_encounter_band(realm);
  const uint8_t roll = d256();
  const uint8_t count_roll = d256();
  const uint8_t level = world_zone_level(realm, player_level, roll);
  const MonsterType primary = world_zone_monster(realm, roll);
  const MonsterType secondary = world_zone_monster(realm, (uint8_t)(roll + 67));
  const MonsterType elite = b->elite;

  if (count_roll < 180) {
    reset_encounter(MONSTER_LAYOUT_1);
    generate_monster(&encounter.monsters[0], primary, level, C_TIER);
    encounter.monsters[0].id = 'A';
  } else if (count_roll < 235) {
    reset_encounter(MONSTER_LAYOUT_2);
    generate_monster(&encounter.monsters[0], primary, level, C_TIER);
    encounter.monsters[0].id = 'A';
    generate_monster(&encounter.monsters[1], secondary, level, B_TIER);
    encounter.monsters[1].id = primary == secondary ? 'B' : 'A';
  } else {
    reset_encounter(MONSTER_LAYOUT_1M_2S);
    generate_monster(&encounter.monsters[0], elite, level, A_TIER);
    encounter.monsters[0].id = 'A';
    generate_monster(&encounter.monsters[1], primary, level, C_TIER);
    encounter.monsters[1].id = 'A';
    generate_monster(&encounter.monsters[2], secondary, level, C_TIER);
    encounter.monsters[2].id = 'B';
  }
  return true;
}
