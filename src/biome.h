/**
 * Biomes describe the environment a floor belongs to. A biome carries the
 * dungeon tileset and palette used for rendering, the roster of monsters that
 * can appear, and the level range encounters should be generated within.
 *
 * This is the layer that "zones" are built from: a zone is a biome plus a set
 * of floors. Keeping the environment data separate from the floor layout means
 * a new area can reuse an existing biome without duplicating any art or
 * encounter tables.
 */
#ifndef _BIOME_H
#define _BIOME_H

#include "biome_id.h"
#include "core.h"
#include "encounter.h"
#include "monster.h"

/**
 * Number of monster slots in a biome roster. Encounters pick from these.
 */
#define BIOME_ROSTER_LEN 6

/**
 * Environment description shared by every floor in a zone.
 */
typedef struct Biome
{
  /**
   * Unique id for the biome.
   */
  BiomeId id;
  /**
   * Display name for the biome, shown on the map menu.
   */
  const char *name;
  /**
   * Dungeon tileset used to render floors in this biome.
   */
  const Tileset *tileset;
  /**
   * Palette applied to the tileset.
   */
  const palette_color_t *palette;
  /**
   * Monsters that can be rolled for an encounter in this biome.
   */
  const MonsterType *roster;
  /**
   * Number of valid entries in `roster`.
   */
  uint8_t roster_len;
  /**
   * Lowest monster level encounters should generate.
   */
  uint8_t min_level;
  /**
   * Highest monster level encounters should generate.
   */
  uint8_t max_level;
  /**
   * Tiers that can appear, as a bitmask indexed by `PowerTier`.
   */
  uint8_t tier_mask;
} Biome;

/**
 * All biomes, indexed by `BiomeId`.
 */
extern const Biome biomes[BIOME_COUNT];

/**
 * @param id Biome to look up.
 * @return The biome for the given id, or `biomes[0]` if the id is unknown.
 */
const Biome *get_biome(BiomeId id) BANKED;

/**
 * Picks a monster type from a biome's roster.
 * @param biome Biome to pick from.
 * @return A monster type from the biome roster.
 */
MonsterType biome_roll_monster(const Biome *biome) BANKED;

/**
 * Picks a level within the biome's level range.
 * @param biome Biome to pick from.
 * @return A level between the biome's min and max level, inclusive.
 */
uint8_t biome_roll_level(const Biome *biome) BANKED;

/**
 * Picks a power tier that the biome allows.
 * @param biome Biome to pick from.
 * @return One of the tiers enabled in the biome's tier mask.
 */
PowerTier biome_roll_tier(const Biome *biome) BANKED;

#endif
