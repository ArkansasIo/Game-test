#ifndef _WORLD_ZONE_ENCOUNTERS_H
#define _WORLD_ZONE_ENCOUNTERS_H
#include <stdint.h>
#include "core.h"
#include "monster.h"
#include "realm_zones.h"
typedef struct WorldEncounterBand {
  RealmZoneId realm;
  uint8_t min_level;
  uint8_t max_level;
  MonsterType primary;
  MonsterType secondary;
  MonsterType elite;
} WorldEncounterBand;
const WorldEncounterBand *world_encounter_band(RealmZoneId realm) BANKED;
MonsterType world_zone_monster(RealmZoneId realm, uint8_t roll) BANKED;
uint8_t world_zone_level(RealmZoneId realm, uint8_t player_level, uint8_t roll) BANKED;
bool generate_world_encounter(uint8_t player_level) BANKED;
#endif
