#pragma bank 7
#include "world_regions.h"
static const WorldRegion regions[WORLD_REGION_COUNT] = {
  { WORLD_REGION_CROWN, "Crownlands", 1, 8, BIOME_DUNGEON_STONE },
  { WORLD_REGION_GREENVALE, "Greenvale", 4, 18, BIOME_DUNGEON_STONE },
  { WORLD_REGION_SUNSCAR, "Sunscar", 12, 30, BIOME_CAVERN },
  { WORLD_REGION_IRONRIDGE, "Ironridge", 18, 42, BIOME_CAVERN },
  { WORLD_REGION_MISTMARSH, "Mistmarsh", 28, 55, BIOME_CRYPT },
  { WORLD_REGION_FROSTFANG, "Frostfang", 40, 70, BIOME_CAVERN },
  { WORLD_REGION_DRAGONWASTE, "Dragon Waste", 55, 99, BIOME_DRAGONS_LAIR },
  { WORLD_REGION_DEEP_OCEAN, "Deep Ocean", 1, 99, BIOME_CAVERN }
};
WorldRegionId world_region_at(uint8_t cx, uint8_t cy) BANKED {
  const uint8_t dx = cx > WORLD_W / 2 ? cx - WORLD_W / 2 : WORLD_W / 2 - cx;
  const uint8_t dy = cy > WORLD_H / 2 ? cy - WORLD_H / 2 : WORLD_H / 2 - cy;
  const uint8_t ring = dx > dy ? dx : dy;
  if (cx == 0 || cy == 0 || cx == WORLD_W - 1 || cy == WORLD_H - 1) return WORLD_REGION_DEEP_OCEAN;
  if (ring <= 2) return WORLD_REGION_CROWN;
  if (cy < WORLD_H / 2 && cx < WORLD_W / 2) return WORLD_REGION_GREENVALE;
  if (cy < WORLD_H / 2) return WORLD_REGION_FROSTFANG;
  if (cx < WORLD_W / 2) return WORLD_REGION_MISTMARSH;
  if (ring >= 6) return WORLD_REGION_DRAGONWASTE;
  if (cx < WORLD_W / 2 + 2) return WORLD_REGION_IRONRIDGE;
  return WORLD_REGION_SUNSCAR;
}
const WorldRegion *world_region_info(WorldRegionId id) BANKED {
  if (id >= WORLD_REGION_COUNT) id = WORLD_REGION_CROWN;
  return &regions[id];
}
