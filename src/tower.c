#pragma bank 7

/**
 * Procedural 100-floor tower.
 *
 * Layout strategy: scatter non-overlapping rectangular rooms, then connect each
 * room to the previous one with an L-shaped corridor. This always produces a
 * fully connected floor, which matters because the player must be able to reach
 * the stairs.
 *
 * The generator is seeded from the floor number, so floor 37 always lays out
 * the same way. That keeps the tower stable across a save/load and means a
 * player who retreats and returns finds the floor they left.
 */
#include <rand.h>
#include <stdint.h>

#include "biome.h"
#include "core.h"
#include "map.h"
#include "player.h"
#include "tower.h"

uint8_t tower_floor = 1;
bool in_tower = false;

/**
 * Shared map buffer for the generated floor. Two bytes per tile: the first
 * carries the tile index and attribute, the second is the palette/attribute
 * byte. The map system reads this through `tower_map`.
 *
 * 32 * 32 tiles * 2 bytes = 2 KB, allocated once and reused for every floor.
 */
static uint8_t tower_map_data[TOWER_MAP_W * TOWER_MAP_H * 2];

static Map tower_active_map = {
    0, // id
    0, // bank (unbanked: the buffer lives in RAM)
    tower_map_data,
    TOWER_MAP_W,
    TOWER_MAP_H,
};

/**
 * Deterministic PRNG state. Seeding this from the floor number is what makes a
 * floor reproducible.
 */
static uint16_t tower_seed;

/**
 * Advances the tower PRNG and returns the next value.
 */
static uint8_t tower_rand(void)
{
  // xorshift16: cheap, and good enough for room placement.
  tower_seed ^= tower_seed << 7;
  tower_seed ^= tower_seed >> 9;
  tower_seed ^= tower_seed << 8;
  return (uint8_t)(tower_seed & 0xFF);
}

/**
 * @param n Upper bound (exclusive).
 * @return A pseudo-random value in 0..n-1.
 */
static uint8_t tower_rand_range(uint8_t n)
{
  if (n == 0)
    return 0;
  return tower_rand() % n;
}

/**
 * Writes a single map tile.
 */
static void tower_put(uint8_t x, uint8_t y, uint8_t tile)
{
  uint16_t offset = (uint16_t)(2 * (x + (uint16_t)y * TOWER_MAP_W));
  tower_map_data[offset] = tile;
  tower_map_data[offset + 1] = 0;
}

/**
 * Fills the buffer with solid wall.
 */
static void tower_clear(void)
{
  uint16_t n = TOWER_MAP_W * TOWER_MAP_H;
  for (uint16_t k = 0; k < n; k++)
  {
    tower_map_data[k * 2] = TOWER_TILE_WALL;
    tower_map_data[k * 2 + 1] = 0;
  }
}

/**
 * Carves a filled rectangle of floor tiles.
 */
static void tower_carve_room(
    uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
  for (uint8_t ry = y; ry < y + h; ry++)
    for (uint8_t rx = x; rx < x + w; rx++)
      tower_put(rx, ry, TOWER_TILE_FLOOR);
}

/**
 * Carves a one-tile-wide horizontal corridor.
 */
static void tower_carve_h(uint8_t x1, uint8_t x2, uint8_t y)
{
  uint8_t lo = x1 < x2 ? x1 : x2;
  uint8_t hi = x1 < x2 ? x2 : x1;
  for (uint8_t x = lo; x <= hi; x++)
    tower_put(x, y, TOWER_TILE_FLOOR);
}

/**
 * Carves a one-tile-wide vertical corridor.
 */
static void tower_carve_v(uint8_t y1, uint8_t y2, uint8_t x)
{
  uint8_t lo = y1 < y2 ? y1 : y2;
  uint8_t hi = y1 < y2 ? y2 : y1;
  for (uint8_t y = lo; y <= hi; y++)
    tower_put(x, y, TOWER_TILE_FLOOR);
}

/**
 * Room centres for the floor currently being generated.
 */
static uint8_t room_cx[TOWER_ROOMS];
static uint8_t room_cy[TOWER_ROOMS];

/**
 * Stair positions for the floor currently loaded. A position of 0 means the
 * floor has no stair of that direction (floor 1 has no down stairs, the top
 * floor has no up stairs).
 */
static uint8_t up_stairs_x, up_stairs_y;
static uint8_t down_stairs_x, down_stairs_y;

/**
 * Scatters rooms and connects them. Rooms are kept inside a one-tile border so
 * the player can never walk off the edge of the map.
 */
static void tower_lay_out_rooms(void)
{
  const uint8_t min_room = 3;
  const uint8_t max_room = 7;

  for (uint8_t r = 0; r < TOWER_ROOMS; r++)
  {
    uint8_t w = min_room + tower_rand_range(max_room - min_room);
    uint8_t h = min_room + tower_rand_range(max_room - min_room);

    // Keep a one-tile wall border around the whole map.
    uint8_t x = 1 + tower_rand_range(TOWER_MAP_W - w - 2);
    uint8_t y = 1 + tower_rand_range(TOWER_MAP_H - h - 2);

    tower_carve_room(x, y, w, h);

    room_cx[r] = x + (w >> 1);
    room_cy[r] = y + (h >> 1);

    // Connect this room to the previous one so the floor is always connected.
    if (r > 0)
    {
      uint8_t px = room_cx[r - 1];
      uint8_t py = room_cy[r - 1];

      if (tower_rand() & 1)
      {
        tower_carve_h(px, room_cx[r], py);
        tower_carve_v(py, room_cy[r], room_cx[r]);
      }
      else
      {
        tower_carve_v(py, room_cy[r], px);
        tower_carve_h(px, room_cx[r], room_cy[r]);
      }
    }
  }
}

/**
 * Places the up stairs in the last room and the down stairs in the first.
 * Both are written as special tiles so the floor's `on_special` callback can
 * decide what happens when the player steps on them.
 */
static void tower_place_stairs(void) {
  up_stairs_x = 0;
  up_stairs_y = 0;
  down_stairs_x = 0;
  down_stairs_y = 0;

  // Down stairs in the first room, unless we are on the ground floor.
  if (tower_floor > 1) {
    down_stairs_x = room_cx[0];
    down_stairs_y = room_cy[0];
    tower_put(down_stairs_x, down_stairs_y, TOWER_TILE_SPECIAL);
  }

  // Up stairs in the last room, unless we are at the top.
  if (tower_floor < TOWER_FLOORS) {
    up_stairs_x = room_cx[TOWER_ROOMS - 1];
    up_stairs_y = room_cy[TOWER_ROOMS - 1];
    tower_put(up_stairs_x, up_stairs_y, TOWER_TILE_SPECIAL);
  }
}

uint8_t tower_up_stairs_x(void) BANKED { return up_stairs_x; }
uint8_t tower_up_stairs_y(void) BANKED { return up_stairs_y; }
uint8_t tower_down_stairs_x(void) BANKED { return down_stairs_x; }
uint8_t tower_down_stairs_y(void) BANKED { return down_stairs_y; }

bool tower_on_special(void) BANKED {
  // hero_x()/hero_y() are signed because the hero can be partially off the
  // map. Cast to uint8_t only after confirming the position is non-negative,
  // so the stair comparisons stay unsigned like the stored coordinates.
  int8_t hx = hero_x();
  int8_t hy = hero_y();
  if (hx < 0 || hy < 0)
    return false;

  uint8_t x = (uint8_t)hx;
  uint8_t y = (uint8_t)hy;

  if (up_stairs_x && x == up_stairs_x && y == up_stairs_y) {
    if (tower_ascend()) {
      // Regenerating a floor invalidates the player's position, so drop them
      // on the new floor's down stairs (or its first room on floor 1).
      set_hero_position(
        down_stairs_x ? down_stairs_x : room_cx[0],
        down_stairs_y ? down_stairs_y : room_cy[0]
      );
    }
    return true;
  }

  if (down_stairs_x && x == down_stairs_x && y == down_stairs_y) {
    if (tower_descend()) {
      set_hero_position(
        up_stairs_x ? up_stairs_x : room_cx[TOWER_ROOMS - 1],
        up_stairs_y ? up_stairs_y : room_cy[TOWER_ROOMS - 1]
      );
    } else {
      // No floor below: leaving the tower from the ground floor.
      tower_leave();
    }
    return true;
  }

  return false;
}

/**
 * Maps a tower floor onto a biome. The tower is a ladder through every biome,
 * so the environment changes as the player climbs.
 */
BiomeId tower_biome_for_floor(uint8_t floor) BANKED
{
  if (floor <= 20)
    return BIOME_DUNGEON_STONE;
  if (floor <= 40)
    return BIOME_CAVERN;
  if (floor <= 60)
    return BIOME_CRYPT;
  if (floor <= 80)
    return BIOME_ARCANE_HALLS;
  return BIOME_DRAGONS_LAIR;
}

/**
 * @param floor Floor number.
 * @return A seed that depends only on the floor number.
 */
static uint16_t tower_seed_for(uint8_t floor)
{
  // Any non-zero value works; 0 would make xorshift produce all zeroes.
  return (uint16_t)(0x9E37u * (uint16_t)(floor + 1));
}

void tower_generate_floor(uint8_t floor) BANKED
{
  if (floor < 1)
    floor = 1;
  if (floor > TOWER_FLOORS)
    floor = TOWER_FLOORS;

  tower_floor = floor;
  tower_seed = tower_seed_for(floor);

  tower_clear();
  tower_lay_out_rooms();
  tower_place_stairs();
}

Map *tower_map(void) BANKED
{
  return &tower_active_map;
}

void tower_enter(void) BANKED
{
  in_tower = true;
  tower_generate_floor(1);
}

void tower_leave(void) BANKED
{
  in_tower = false;
}

bool tower_ascend(void) BANKED
{
  if (tower_floor >= TOWER_FLOORS)
    return false;
  tower_generate_floor(tower_floor + 1);
  return true;
}

bool tower_descend(void) BANKED
{
  if (tower_floor <= 1)
    return false;
  tower_generate_floor(tower_floor - 1);
  return true;
}
