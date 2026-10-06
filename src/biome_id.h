/**
 * Biome identifiers.
 *
 * This lives in its own header, separate from `biome.h`, because `map.h` needs
 * to store a biome id on every Floor. `biome.h` pulls in the encounter and
 * player headers, which include `map.h` again, so `map.h` cannot include
 * `biome.h` directly without creating an include cycle. Keeping the enum here
 * lets both sides share it with no dependencies.
 */
#ifndef _BIOME_ID_H
#define _BIOME_ID_H

/**
 * Identifies a biome. Biomes are referenced by id so floors stay small.
 */
typedef enum BiomeId
{
  BIOME_DUNGEON_STONE,
  BIOME_CAVERN,
  BIOME_CRYPT,
  BIOME_ARCANE_HALLS,
  BIOME_DRAGONS_LAIR,
  BIOME_COUNT,
  BIOME_NONE = 0xFF,
} BiomeId;

#endif
