#pragma bank 7
#include "realm_zones.h"

static const RealmZone zones[REALM_ZONE_COUNT] = {
 { REALM_ZONE_CROWN_REALM, "Crown Realm", "The royal heartland and beginning of the quest.", 1, 8, BIOME_DUNGEON_STONE, REALM_ZONE_SAFE },
 { REALM_ZONE_GREEN_REALM, "Green Realm", "Ancient forests, farms and hidden fairy roads.", 4, 18, BIOME_DUNGEON_STONE, 0 },
 { REALM_ZONE_SUN_REALM, "Sun Realm", "Burning plains, red deserts and forgotten ruins.", 12, 30, BIOME_CAVERN, REALM_ZONE_DANGEROUS },
 { REALM_ZONE_IRON_REALM, "Iron Realm", "Mountain kingdoms, mines and fortified passes.", 18, 42, BIOME_CAVERN, REALM_ZONE_DANGEROUS },
 { REALM_ZONE_MIST_REALM, "Mist Realm", "Cursed marshes where the dead refuse to sleep.", 28, 55, BIOME_CRYPT, REALM_ZONE_DANGEROUS },
 { REALM_ZONE_FROST_REALM, "Frost Realm", "Frozen citadels and the northern dragon roads.", 40, 70, BIOME_CAVERN, REALM_ZONE_DANGEROUS },
 { REALM_ZONE_SHADOW_REALM, "Shadow Realm", "A twilight land of demons and corrupted towers.", 50, 80, BIOME_CRYPT, REALM_ZONE_DANGEROUS },
 { REALM_ZONE_DRAGON_REALM, "Dragon Realm", "Volcanic territory ruled by ancient dragons.", 55, 99, BIOME_DRAGONS_LAIR, REALM_ZONE_BOSS | REALM_ZONE_ENDGAME },
 { REALM_ZONE_ASTRAL_REALM, "Astral Realm", "A hidden sky realm reached beyond the mortal world.", 65, 99, BIOME_ARCANE_HALLS, REALM_ZONE_SECRET | REALM_ZONE_ENDGAME },
 { REALM_ZONE_VOID_REALM, "Void Realm", "The final forbidden zone beyond the world's edge.", 80, 99, BIOME_DRAGONS_LAIR, REALM_ZONE_SECRET | REALM_ZONE_BOSS | REALM_ZONE_ENDGAME }
};

RealmZoneId realm_zone_at(uint8_t cx, uint8_t cy) BANKED {
  const uint8_t center = WORLD_W / 2;
  const uint8_t dx = cx > center ? cx - center : center - cx;
  const uint8_t dy = cy > center ? cy - center : center - cy;
  const uint8_t ring = dx > dy ? dx : dy;

  if (cx == 0 || cy == 0 || cx >= WORLD_W - 1 || cy >= WORLD_H - 1)
    return REALM_ZONE_VOID_REALM;
  if (ring <= 2) return REALM_ZONE_CROWN_REALM;
  if (ring >= 7) return REALM_ZONE_DRAGON_REALM;
  if (cy < center && cx < center) return REALM_ZONE_GREEN_REALM;
  if (cy < center) return REALM_ZONE_FROST_REALM;
  if (cx < center) return REALM_ZONE_MIST_REALM;
  if (ring == 6) return REALM_ZONE_SHADOW_REALM;
  if (cx < center + 2) return REALM_ZONE_IRON_REALM;
  return REALM_ZONE_SUN_REALM;
}

const RealmZone *realm_zone_info(RealmZoneId id) BANKED {
  if (id >= REALM_ZONE_COUNT) id = REALM_ZONE_CROWN_REALM;
  return &zones[id];
}

uint8_t realm_zone_is_blocked(RealmZoneId id) BANKED {
  return id == REALM_ZONE_VOID_REALM;
}
