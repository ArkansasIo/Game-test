#pragma bank 7

/**
 * Chunked overworld implementation.
 *
 * World layout is derived, not stored. `world_kind_at` is a pure function of
 * the chunk coordinates, so the same coordinates always produce the same kind
 * of place and the same layout. That is what lets a 16x16 world exist on a
 * cartridge that could never hold it as data.
 *
 * The chunk the player occupies is written into a single shared RAM buffer
 * (the same 2 KB pattern the tower uses) and handed to the map system as the
 * active map. Travelling regenerates the buffer and repositions the player.
 */
#include <rand.h>
#include <stdint.h>

#include "biome.h"
#include "core.h"
#include "map.h"
#include "strings.h"
#include "tower.h"
#include "world.h"

uint8_t world_chunk_x = 0;
uint8_t world_chunk_y = 0;
bool in_world = false;
ChunkKind current_chunk_kind = CHUNK_WILDERNESS;

/**
 * Shared map buffer for the active chunk: two bytes per tile, the same
 * encoding the map system reads (`[tile | attribute][palette]`).
 */
static uint8_t world_map_data[WORLD_CHUNK_W * WORLD_CHUNK_H * 2];

static Map world_active_map = {
    0, // id
    0, // bank (unbanked: the buffer lives in RAM)
    world_map_data,
    WORLD_CHUNK_W,
    WORLD_CHUNK_H,
};

/**
 * Deterministic PRNG state for the chunk being generated.
 */
static uint16_t world_seed;

/**
 * Advances the world PRNG.
 */
static uint8_t world_rand(void)
{
  world_seed ^= world_seed << 7;
  world_seed ^= world_seed >> 9;
  world_seed ^= world_seed << 8;
  return (uint8_t)(world_seed & 0xFF);
}

/**
 * @param n Upper bound (exclusive).
 * @return A pseudo-random value in 0..n-1.
 */
static uint8_t world_rand_range(uint8_t n)
{
  if (n == 0)
    return 0;
  return world_rand() % n;
}

//------------------------------------------------------------------------------
// Map buffer helpers
//------------------------------------------------------------------------------

static void world_put(uint8_t x, uint8_t y, uint8_t tile)
{
  uint16_t offset = (uint16_t)(2 * (x + (uint16_t)y * WORLD_CHUNK_W));
  world_map_data[offset] = tile;
  world_map_data[offset + 1] = 0;
}

static void world_fill(uint8_t tile)
{
  uint16_t n = WORLD_CHUNK_W * WORLD_CHUNK_H;
  for (uint16_t k = 0; k < n; k++)
  {
    world_map_data[k * 2] = tile;
    world_map_data[k * 2 + 1] = 0;
  }
}

static void world_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t t)
{
  for (uint8_t ry = y; ry < y + h; ry++)
    for (uint8_t rx = x; rx < x + w; rx++)
      world_put(rx, ry, t);
}

static void world_hline(uint8_t x1, uint8_t x2, uint8_t y, uint8_t t)
{
  uint8_t lo = x1 < x2 ? x1 : x2;
  uint8_t hi = x1 < x2 ? x2 : x1;
  for (uint8_t x = lo; x <= hi; x++)
    world_put(x, y, t);
}

static void world_vline(uint8_t y1, uint8_t y2, uint8_t x, uint8_t t)
{
  uint8_t lo = y1 < y2 ? y1 : y2;
  uint8_t hi = y1 < y2 ? y2 : y1;
  for (uint8_t y = lo; y <= hi; y++)
    world_put(x, y, t);
}

//------------------------------------------------------------------------------
// World shape
//------------------------------------------------------------------------------

/**
 * Hashes chunk coordinates into a stable value. Used to derive terrain without
 * storing it.
 */
static uint16_t world_hash(uint8_t cx, uint8_t cy)
{
  uint16_t h = (uint16_t)(cx * 73u) ^ (uint16_t)(cy * 151u);
  h ^= h >> 5;
  h = (uint16_t)(h * 0x9E37u);
  h ^= h >> 7;
  return h;
}

ChunkKind world_kind_at(uint8_t cx, uint8_t cy) BANKED
{
  // Keep a border of ocean so the player cannot walk off the world.
  if (cx == 0 || cy == 0 || cx >= WORLD_W - 1 || cy >= WORLD_H - 1)
    return CHUNK_OCEAN;

  // The centre of the map is always the starting settlement.
  if (cx == WORLD_W / 2 && cy == WORLD_H / 2)
    return CHUNK_CITY;

  // Ring the city with towns, and those with villages, so the player meets
  // settlements in a sensible order as they move outwards.
  const uint8_t dx =
      cx > WORLD_W / 2 ? cx - WORLD_W / 2 : WORLD_W / 2 - cx;
  const uint8_t dy =
      cy > WORLD_H / 2 ? cy - WORLD_H / 2 : WORLD_H / 2 - cy;
  const uint8_t ring = dx > dy ? dx : dy;

  const uint16_t h = world_hash(cx, cy);

  if (ring == 1)
    return CHUNK_TOWN;
  if (ring == 2)
    return (h & 3) == 0 ? CHUNK_VILLAGE : CHUNK_WILDERNESS;

  // Farther out: mostly wilderness, with landmarks scattered through it.
  switch (h & 7)
  {
  case 0:
    return CHUNK_VILLAGE;
  case 1:
    return CHUNK_DUNGEON;
  case 2:
  case 3:
    return CHUNK_MOUNTAIN;
  default:
    return CHUNK_WILDERNESS;
  }
}

BiomeId world_biome_at(uint8_t cx, uint8_t cy) BANKED
{
  switch (world_kind_at(cx, cy))
  {
  case CHUNK_CITY:
  case CHUNK_TOWN:
  case CHUNK_VILLAGE:
    return BIOME_DUNGEON_STONE;
  case CHUNK_DUNGEON:
    return BIOME_CRYPT;
  case CHUNK_MOUNTAIN:
    return BIOME_CAVERN;
  case CHUNK_OCEAN:
    return BIOME_CAVERN;
  default:
  {
    // Wilderness biome varies by region so the map is not uniform.
    const uint16_t h = world_hash(cx, cy);
    switch ((h >> 4) & 3)
    {
    case 0:
      return BIOME_DUNGEON_STONE;
    case 1:
      return BIOME_CAVERN;
    case 2:
      return BIOME_CRYPT;
    default:
      return BIOME_ARCANE_HALLS;
    }
  }
  }
}

const char *world_kind_name(ChunkKind kind) BANKED {
  switch (kind) {
  case CHUNK_VILLAGE:
    return str_maps_world_village;
  case CHUNK_TOWN:
    return str_maps_world_town;
  case CHUNK_CITY:
    return str_maps_world_city;
  case CHUNK_DUNGEON:
    return str_maps_world_dungeon;
  case CHUNK_OCEAN:
    return str_maps_world_ocean;
  case CHUNK_MOUNTAIN:
    return str_maps_world_mountain;
  default:
    return str_maps_world_wilderness;
  }
}

//------------------------------------------------------------------------------
// Chunk layouts
//------------------------------------------------------------------------------

/**
 * Lays out a settlement. Buildings are solid blocks with a door gap on the
 * side facing the plaza, arranged in a ring around a central well.
 */
static void world_lay_out_settlement(uint8_t buildings)
{
  world_fill(TOWER_TILE_FLOOR);

  // Border of impassable rock so the player leaves via the chunk edges.
  world_hline(0, WORLD_CHUNK_W - 1, 0, TOWER_TILE_WALL);
  world_hline(0, WORLD_CHUNK_W - 1, WORLD_CHUNK_H - 1, TOWER_TILE_WALL);
  world_vline(0, WORLD_CHUNK_H - 1, 0, TOWER_TILE_WALL);
  world_vline(0, WORLD_CHUNK_H - 1, WORLD_CHUNK_W - 1, TOWER_TILE_WALL);

  // Leave gaps in the middle of each edge so the player can travel out.
  const uint8_t mid_x = WORLD_CHUNK_W / 2;
  const uint8_t mid_y = WORLD_CHUNK_H / 2;
  world_put(mid_x, 0, TOWER_TILE_FLOOR);
  world_put(mid_x, WORLD_CHUNK_H - 1, TOWER_TILE_FLOOR);
  world_put(0, mid_y, TOWER_TILE_FLOOR);
  world_put(WORLD_CHUNK_W - 1, mid_y, TOWER_TILE_FLOOR);

  // Buildings around a plaza.
  const uint8_t bw = 4;
  const uint8_t bh = 3;
  const uint8_t step_x = 7;
  const uint8_t step_y = 6;

  for (uint8_t b = 0; b < buildings; b++)
  {
    uint8_t bx = 4 + (b % 3) * step_x;
    uint8_t by = 5 + (b / 3) * step_y;

    if (bx + bw >= WORLD_CHUNK_W - 1 || by + bh >= WORLD_CHUNK_H - 1)
      continue;

    world_rect(bx, by, bw, bh, TOWER_TILE_WALL);

    // Door facing the centre of the chunk, so the plaza stays reachable.
    if (by < mid_y)
      world_put(bx + (bw >> 1), by + bh - 1, TOWER_TILE_FLOOR);
    else
      world_put(bx + (bw >> 1), by, TOWER_TILE_FLOOR);
  }
}

/**
 * Lays out wilderness: mostly open, with scattered obstacles and a water
 * feature. Obstacles never fully block a row or column, so the chunk stays
 * crossable.
 */
static void world_lay_out_wilderness(void)
{
  world_fill(TOWER_TILE_FLOOR);

  world_hline(0, WORLD_CHUNK_W - 1, 0, TOWER_TILE_WALL);
  world_hline(0, WORLD_CHUNK_W - 1, WORLD_CHUNK_H - 1, TOWER_TILE_WALL);
  world_vline(0, WORLD_CHUNK_H - 1, 0, TOWER_TILE_WALL);
  world_vline(0, WORLD_CHUNK_H - 1, WORLD_CHUNK_W - 1, TOWER_TILE_WALL);

  const uint8_t mid_x = WORLD_CHUNK_W / 2;
  const uint8_t mid_y = WORLD_CHUNK_H / 2;
  world_put(mid_x, 0, TOWER_TILE_FLOOR);
  world_put(mid_x, WORLD_CHUNK_H - 1, TOWER_TILE_FLOOR);
  world_put(0, mid_y, TOWER_TILE_FLOOR);
  world_put(WORLD_CHUNK_W - 1, mid_y, TOWER_TILE_FLOOR);

  // Scatter small clumps of rock and trees.
  const uint8_t clumps = 12;
  for (uint8_t c = 0; c < clumps; c++)
  {
    uint8_t x = 2 + world_rand_range(WORLD_CHUNK_W - 4);
    uint8_t y = 2 + world_rand_range(WORLD_CHUNK_H - 4);

    // Never wall off the central cross, which the edge gaps feed into.
    if (x == mid_x || y == mid_y)
      continue;

    world_put(x, y, TOWER_TILE_WALL);
  }

  // A small pond.
  if (world_rand() & 1)
  {
    uint8_t px = 5 + world_rand_range(WORLD_CHUNK_W - 14);
    uint8_t py = 5 + world_rand_range(WORLD_CHUNK_H - 14);
    if (px != mid_x && py != mid_y)
      world_rect(px, py, 4, 3, TOWER_TILE_WALL);
  }
}

/**
 * Lays out a dungeon chunk: the room-and-corridor style, with a single
 * landmark in the middle room.
 */
static void world_lay_out_dungeon(void) {
  world_fill(TOWER_TILE_WALL);

  const uint8_t rooms = 4;
  uint8_t cx[4];
  uint8_t cy[4];

  for (uint8_t r = 0; r < rooms; r++) {
    uint8_t w = 4 + world_rand_range(4);
    uint8_t h = 4 + world_rand_range(4);
    uint8_t x = 2 + world_rand_range(WORLD_CHUNK_W - w - 4);
    uint8_t y = 2 + world_rand_range(WORLD_CHUNK_H - h - 4);

    world_rect(x, y, w, h, TOWER_TILE_FLOOR);

    cx[r] = x + (w >> 1);
    cy[r] = y + (h >> 1);

    if (r > 0) {
      if (world_rand() & 1) {
        world_hline(cx[r - 1], cx[r], cy[r - 1], TOWER_TILE_FLOOR);
        world_vline(cy[r - 1], cy[r], cx[r], TOWER_TILE_FLOOR);
      } else {
        world_vline(cy[r - 1], cy[r], cx[r - 1], TOWER_TILE_FLOOR);
        world_hline(cx[r - 1], cx[r], cy[r], TOWER_TILE_FLOOR);
      }
    }
  }

  // Carve a cross through the chunk that reaches all four edges, then punch
  // the edge gaps into it. The corridors must span the full width/height:
  // a gap in the border row alone opens onto solid rock and the player can
  // never get inside.
  const uint8_t mid_x = WORLD_CHUNK_W / 2;
  const uint8_t mid_y = WORLD_CHUNK_H / 2;

  world_vline(0, WORLD_CHUNK_H - 1, mid_x, TOWER_TILE_FLOOR);
  world_hline(0, WORLD_CHUNK_W - 1, mid_y, TOWER_TILE_FLOOR);

  // The cross alone would leave the four corners walled off, which is fine,
  // but every room must touch it or the floor is not fully traversable.
  for (uint8_t r = 0; r < rooms; r++) {
    world_vline(cy[r], mid_y, cx[r], TOWER_TILE_FLOOR);
    world_hline(cx[r], mid_x, cy[r], TOWER_TILE_FLOOR);
  }
}

/**
 * Lays out impassable terrain. Kept visually distinct so the player can tell
 * at a glance that it is not walkable.
 */
static void world_lay_out_blocked(void)
{
  world_fill(TOWER_TILE_WALL);
}

/**
 * @param cx Chunk column.
 * @param cy Chunk row.
 * @return A seed that depends only on the chunk coordinates.
 */
static uint16_t world_seed_for(uint8_t cx, uint8_t cy)
{
  return world_hash(cx, cy) | 1u; // never zero: xorshift would lock up
}

void world_generate_chunk(uint8_t cx, uint8_t cy) BANKED
{
  if (cx >= WORLD_W)
    cx = WORLD_W - 1;
  if (cy >= WORLD_H)
    cy = WORLD_H - 1;

  world_chunk_x = cx;
  world_chunk_y = cy;
  world_seed = world_seed_for(cx, cy);
  current_chunk_kind = world_kind_at(cx, cy);

  switch (current_chunk_kind)
  {
  case CHUNK_CITY:
    world_lay_out_settlement(9);
    break;
  case CHUNK_TOWN:
    world_lay_out_settlement(6);
    break;
  case CHUNK_VILLAGE:
    world_lay_out_settlement(3);
    break;
  case CHUNK_DUNGEON:
    world_lay_out_dungeon();
    break;
  case CHUNK_OCEAN:
  case CHUNK_MOUNTAIN:
    world_lay_out_blocked();
    break;
  default:
    world_lay_out_wilderness();
    break;
  }
}

Map *world_map(void) BANKED
{
  return &world_active_map;
}

void world_enter(void) BANKED
{
  in_world = true;
  world_generate_chunk(WORLD_W / 2, WORLD_H / 2);
}

bool world_travel(int8_t dx, int8_t dy) BANKED
{
  int16_t nx = (int16_t)world_chunk_x + dx;
  int16_t ny = (int16_t)world_chunk_y + dy;

  if (nx < 0 || ny < 0 || nx >= WORLD_W || ny >= WORLD_H)
    return false;

  // Impassable chunks cannot be entered.
  ChunkKind kind = world_kind_at((uint8_t)nx, (uint8_t)ny);
  if (kind == CHUNK_OCEAN || kind == CHUNK_MOUNTAIN)
    return false;

  world_generate_chunk((uint8_t)nx, (uint8_t)ny);

  // Place the player on the opposite edge of the new chunk, so the transition
  // reads as walking straight across the boundary.
  const int8_t mid = WORLD_CHUNK_W / 2;
  if (dx > 0)
    set_hero_position(1, mid);
  else if (dx < 0)
    set_hero_position(WORLD_CHUNK_W - 2, mid);
  else if (dy > 0)
    set_hero_position(mid, 1);
  else if (dy < 0)
    set_hero_position(mid, WORLD_CHUNK_H - 2);

  return true;
}

bool world_on_move(void) BANKED
{
  const int8_t x = hero_x();
  const int8_t y = hero_y();
  const int8_t max_x = WORLD_CHUNK_W - 1;
  const int8_t max_y = WORLD_CHUNK_H - 1;

  // Walking into an edge gap moves to the adjacent chunk.
  if (x <= 0)
    return world_travel(-1, 0);
  if (x >= max_x)
    return world_travel(1, 0);
  if (y <= 0)
    return world_travel(0, -1);
  if (y >= max_y)
    return world_travel(0, 1);

  return false;
}
