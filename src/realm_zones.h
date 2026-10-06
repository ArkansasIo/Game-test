#ifndef _REALM_ZONES_H
#define _REALM_ZONES_H

#include <stdint.h>
#include "biome_id.h"
#include "core.h"

typedef enum RealmZoneId {
  REALM_ZONE_CROWN_REALM,
  REALM_ZONE_GREEN_REALM,
  REALM_ZONE_SUN_REALM,
  REALM_ZONE_IRON_REALM,
  REALM_ZONE_MIST_REALM,
  REALM_ZONE_FROST_REALM,
  REALM_ZONE_SHADOW_REALM,
  REALM_ZONE_DRAGON_REALM,
  REALM_ZONE_ASTRAL_REALM,
  REALM_ZONE_VOID_REALM,
  REALM_ZONE_COUNT
} RealmZoneId;

typedef struct RealmZone {
  RealmZoneId id;
  const char *name;
  const char *description;
  uint8_t min_level;
  uint8_t max_level;
  BiomeId biome;
  uint8_t flags;
} RealmZone;

#define REALM_ZONE_SAFE       0x01
#define REALM_ZONE_DANGEROUS  0x02
#define REALM_ZONE_BOSS       0x04
#define REALM_ZONE_SECRET     0x08
#define REALM_ZONE_ENDGAME    0x10

RealmZoneId realm_zone_at(uint8_t cx, uint8_t cy) BANKED;
const RealmZone *realm_zone_info(RealmZoneId id) BANKED;
uint8_t realm_zone_is_blocked(RealmZoneId id) BANKED;

#endif
