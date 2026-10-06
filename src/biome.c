#pragma bank 0

#include <rand.h>

#include "biome.h"
#include "data.h"
#include "monster.h"
#include "stats.h"

//------------------------------------------------------------------------------
// Dungeon tileset
//------------------------------------------------------------------------------

/**
 * The dungeon tileset is the shared environment art for every biome. Biomes
 * differentiate themselves through their palette and monster roster rather
 * than through unique tile art, so no biome needs its own tileset.
 */
static const Tileset biome_tileset = {
    128,
    12,
    tile_data_dungeon};

//------------------------------------------------------------------------------
// Palettes
//------------------------------------------------------------------------------

/**
 * Stony dungeon: the neutral grey-brown palette used by floors 1 and 2.
 */
static const palette_color_t biome_palette_stone[4] = {
    RGB8(190, 200, 190),
    RGB8(120, 112, 96),
    RGB8(72, 64, 56),
    RGB8(24, 0, 0),
};

/**
 * Cavern: cooler, damper tones for the mid dungeon.
 */
static const palette_color_t biome_palette_cavern[4] = {
    RGB8(176, 200, 208),
    RGB8(88, 112, 136),
    RGB8(48, 60, 84),
    RGB8(16, 8, 24),
};

/**
 * Crypt: desaturated greens and bone.
 */
static const palette_color_t biome_palette_crypt[4] = {
    RGB8(196, 208, 184),
    RGB8(104, 120, 88),
    RGB8(56, 64, 48),
    RGB8(16, 16, 8),
};

/**
 * Arcane halls: violet and indigo.
 */
static const palette_color_t biome_palette_arcane[4] = {
    RGB8(208, 192, 216),
    RGB8(128, 96, 168),
    RGB8(64, 40, 104),
    RGB8(16, 8, 32),
};

/**
 * Dragon's lair: warm reds and ember.
 */
static const palette_color_t biome_palette_lair[4] = {
    RGB8(216, 192, 168),
    RGB8(168, 88, 56),
    RGB8(96, 32, 24),
    RGB8(24, 8, 8),
};

//------------------------------------------------------------------------------
// Monster rosters
//------------------------------------------------------------------------------

static const MonsterType roster_stone[BIOME_ROSTER_LEN] = {
    MONSTER_KOBOLD,
    MONSTER_GOBLIN,
    MONSTER_ZOMBIE,
    MONSTER_BUGBEAR,
    MONSTER_OWLBEAR,
    MONSTER_DUMMY,
};

static const MonsterType roster_cavern[BIOME_ROSTER_LEN] = {
    MONSTER_GOBLIN,
    MONSTER_BUGBEAR,
    MONSTER_GELATINOUS_CUBE,
    MONSTER_OWLBEAR,
    MONSTER_WILL_O_WISP,
    MONSTER_DISPLACER_BEAST,
};

static const MonsterType roster_crypt[BIOME_ROSTER_LEN] = {
    MONSTER_ZOMBIE,
    MONSTER_GELATINOUS_CUBE,
    MONSTER_DEATHKNIGHT,
    MONSTER_WILL_O_WISP,
    MONSTER_MINDFLAYER,
    MONSTER_DISPLACER_BEAST,
};

static const MonsterType roster_arcane[BIOME_ROSTER_LEN] = {
    MONSTER_WILL_O_WISP,
    MONSTER_MINDFLAYER,
    MONSTER_DEATHKNIGHT,
    MONSTER_BEHOLDER,
    MONSTER_GELATINOUS_CUBE,
    MONSTER_DISPLACER_BEAST,
};

static const MonsterType roster_lair[BIOME_ROSTER_LEN] = {
    MONSTER_DEATHKNIGHT,
    MONSTER_BEHOLDER,
    MONSTER_MINDFLAYER,
    MONSTER_DRAGON,
    MONSTER_DISPLACER_BEAST,
    MONSTER_OWLBEAR,
};

//------------------------------------------------------------------------------
// Biome table
//------------------------------------------------------------------------------

/**
 * Tier bitmasks. Bit N is set when tier N is allowed.
 */
#define TIER_C_ONLY (1 << C_TIER)
#define TIER_C_B ((1 << C_TIER) | (1 << B_TIER))
#define TIER_C_B_A ((1 << C_TIER) | (1 << B_TIER) | (1 << A_TIER))
#define TIER_ALL ((1 << C_TIER) | (1 << B_TIER) | (1 << A_TIER) | (1 << S_TIER))

const Biome biomes[BIOME_COUNT] = {
    {
        BIOME_DUNGEON_STONE,
        str_misc_physical,
        &biome_tileset,
        biome_palette_stone,
        roster_stone,
        BIOME_ROSTER_LEN,
        5,
        14,
        TIER_C_B,
    },
    {
        BIOME_CAVERN,
        str_misc_physical,
        &biome_tileset,
        biome_palette_cavern,
        roster_cavern,
        BIOME_ROSTER_LEN,
        15,
        28,
        TIER_C_B_A,
    },
    {
        BIOME_CRYPT,
        str_misc_physical,
        &biome_tileset,
        biome_palette_crypt,
        roster_crypt,
        BIOME_ROSTER_LEN,
        29,
        44,
        TIER_C_B_A,
    },
    {
        BIOME_ARCANE_HALLS,
        str_misc_physical,
        &biome_tileset,
        biome_palette_arcane,
        roster_arcane,
        BIOME_ROSTER_LEN,
        45,
        59,
        TIER_C_B_A,
    },
    {
        BIOME_DRAGONS_LAIR,
        str_misc_physical,
        &biome_tileset,
        biome_palette_lair,
        roster_lair,
        BIOME_ROSTER_LEN,
        60,
        MAX_LEVEL,
        TIER_ALL,
    },
};

//------------------------------------------------------------------------------
// Queries
//------------------------------------------------------------------------------

const Biome *get_biome(BiomeId id) BANKED
{
  if (id >= BIOME_COUNT)
    return biomes;
  return biomes + id;
}

MonsterType biome_roll_monster(const Biome *biome) BANKED
{
  if (biome->roster_len == 0)
    return MONSTER_KOBOLD;
  return biome->roster[d8() % biome->roster_len];
}

uint8_t biome_roll_level(const Biome *biome) BANKED
{
  if (biome->max_level <= biome->min_level)
    return biome->min_level;
  uint8_t span = biome->max_level - biome->min_level + 1;
  return biome->min_level + (d256() % span);
}

PowerTier biome_roll_tier(const Biome *biome) BANKED
{
  // Collect the allowed tiers, then pick one at random. This keeps the roll
  // uniform across the allowed tiers rather than favouring low bits.
  PowerTier allowed[4];
  uint8_t count = 0;

  for (uint8_t k = 0; k < 4; k++)
  {
    if (biome->tier_mask & (1 << k))
      allowed[count++] = (PowerTier)k;
  }

  if (count == 0)
    return C_TIER;

  return allowed[d8() % count];
}
