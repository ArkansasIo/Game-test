#ifndef _WORLD_REGIONS_H
#define _WORLD_REGIONS_H
#include <stdint.h>
#include "biome_id.h"
#include "world.h"
typedef enum WorldRegionId {
  WORLD_REGION_CROWN, WORLD_REGION_GREENVALE, WORLD_REGION_SUNSCAR, WORLD_REGION_IRONRIDGE,
  WORLD_REGION_MISTMARSH, WORLD_REGION_FROSTFANG, WORLD_REGION_DRAGONWASTE, WORLD_REGION_DEEP_OCEAN,
  WORLD_REGION_COUNT
} WorldRegionId;
typedef struct WorldRegion {
  WorldRegionId id;
  const char *name;
  uint8_t min_level;
  uint8_t max_level;
  BiomeId biome;
} WorldRegion;
WorldRegionId world_region_at(uint8_t cx, uint8_t cy) BANKED;
const WorldRegion *world_region_info(WorldRegionId id) BANKED;
#endif
