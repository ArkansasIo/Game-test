#pragma bank 7
#include <rand.h>
#include <stdint.h>
#include "biome.h"
#include "core.h"
#include "map.h"
#include "strings.h"
#include "tower.h"
#include "world.h"
#include "world_landmarks.h"
#include "world_regions.h"

uint8_t world_chunk_x = 0;
uint8_t world_chunk_y = 0;
bool in_world = false;
ChunkKind current_chunk_kind = CHUNK_WILDERNESS;

static uint8_t world_map_data[WORLD_CHUNK_W * WORLD_CHUNK_H * 2];
static Map world_active_map = { 0, 0, world_map_data, WORLD_CHUNK_W, WORLD_CHUNK_H };
static uint16_t world_seed;

static uint8_t world_rand(void) {
  world_seed ^= world_seed << 7;
  world_seed ^= world_seed >> 9;
  world_seed ^= world_seed << 8;
  return (uint8_t)(world_seed & 0xFF);
}

static uint8_t world_rand_range(uint8_t n) {
  if (n == 0) return 0;
  return world_rand() % n;
}

static void world_put(uint8_t x, uint8_t y, uint8_t tile) {
  uint16_t offset = (uint16_t)(2 * (x + (uint16_t)y * WORLD_CHUNK_W));
  world_map_data[offset] = tile;
  world_map_data[offset + 1] = 0;
}

static void world_fill(uint8_t tile) {
  uint16_t n = WORLD_CHUNK_W * WORLD_CHUNK_H;
  for (uint16_t k = 0; k < n; k++) {
    world_map_data[k * 2] = tile;
    world_map_data[k * 2 + 1] = 0;
  }
}

static void world_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t t) {
  for (uint8_t ry = y; ry < y + h; ry++)
    for (uint8_t rx = x; rx < x + w; rx++)
      world_put(rx, ry, t);
}

static void world_hline(uint8_t x1, uint8_t x2, uint8_t y, uint8_t t) {
  uint8_t lo = x1 < x2 ? x1 : x2;
  uint8_t hi = x1 < x2 ? x2 : x1;
  for (uint8_t x = lo; x <= hi; x++) world_put(x, y, t);
}

static void world_vline(uint8_t y1, uint8_t y2, uint8_t x, uint8_t t) {
  uint8_t lo = y1 < y2 ? y1 : y2;
  uint8_t hi = y1 < y2 ? y2 : y1;
  for (uint8_t y = lo; y <= hi; y++) world_put(x, y, t);
}

static uint16_t world_hash(uint8_t cx, uint8_t cy) {
  uint16_t h = (uint16_t)(cx * 73u) ^ (uint16_t)(cy * 151u);
  h ^= h >> 5;
  h = (uint16_t)(h * 0x9E37u);
  h ^= h >> 7;
  return h;
}

ChunkKind world_kind_at(uint8_t cx, uint8_t cy) BANKED {
  if (cx == 0 || cy == 0 || cx >= WORLD_W - 1 || cy >= WORLD_H - 1)
    return CHUNK_OCEAN;
  if (cx == WORLD_W / 2 && cy == WORLD_H / 2)
    return CHUNK_CITY;

  const uint8_t dx = cx > WORLD_W / 2 ? cx - WORLD_W / 2 : WORLD_W / 2 - cx;
  const uint8_t dy = cy > WORLD_H / 2 ? cy - WORLD_H / 2 : WORLD_H / 2 - cy;
  const uint8_t ring = dx > dy ? dx : dy;
  const uint16_t h = world_hash(cx, cy);

  if (ring == 1) return CHUNK_TOWN;
  if (ring == 2) return (h & 3) == 0 ? CHUNK_VILLAGE : CHUNK_WILDERNESS;

  switch (h & 7) {
  case 0: return CHUNK_VILLAGE;
  case 1: return CHUNK_DUNGEON;
  case 2:
  case 3: return CHUNK_MOUNTAIN;
  default: return CHUNK_WILDERNESS;
  }
}

BiomeId world_biome_at(uint8_t cx, uint8_t cy) BANKED {
  return world_region_info(world_region_at(cx, cy))->biome;
}

const char *world_kind_name(ChunkKind kind) BANKED {
  switch (kind) {
  case CHUNK_VILLAGE: return str_maps_world_village;
  case CHUNK_TOWN: return str_maps_world_town;
  case CHUNK_CITY: return str_maps_world_city;
  case CHUNK_DUNGEON: return str_maps_world_dungeon;
  case CHUNK_OCEAN: return str_maps_world_ocean;
  case CHUNK_MOUNTAIN: return str_maps_world_mountain;
  default: return str_maps_world_wilderness;
  }
}

static void world_lay_out_settlement(uint8_t buildings) {
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

  for (uint8_t b = 0; b < buildings; b++) {
    const uint8_t bx = 4 + (b % 3) * 7;
    const uint8_t by = 5 + (b / 3) * 6;
    const bool fits = (bx + 4 < WORLD_CHUNK_W - 1) &&
                      (by + 3 < WORLD_CHUNK_H - 1);
    if (fits) {
      world_rect(bx, by, 4, 3, TOWER_TILE_WALL);
      if (by < mid_y)
        world_put(bx + 2, by + 2, TOWER_TILE_FLOOR);
      else
        world_put(bx + 2, by, TOWER_TILE_FLOOR);
    }
  }
}

static void world_lay_out_wilderness(void) {
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

  for (uint8_t c = 0; c < 12; c++) {
    uint8_t x = 2 + world_rand_range(WORLD_CHUNK_W - 4);
    uint8_t y = 2 + world_rand_range(WORLD_CHUNK_H - 4);
    if (x == mid_x || y == mid_y) continue;
    world_put(x, y, TOWER_TILE_WALL);
  }

  if (world_rand() & 1) {
    uint8_t px = 5 + world_rand_range(WORLD_CHUNK_W - 14);
    uint8_t py = 5 + world_rand_range(WORLD_CHUNK_H - 14);
    if (px != mid_x && py != mid_y) world_rect(px, py, 4, 3, TOWER_TILE_WALL);
  }
}

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

  const uint8_t mid_x = WORLD_CHUNK_W / 2;
  const uint8_t mid_y = WORLD_CHUNK_H / 2;
  world_vline(0, WORLD_CHUNK_H - 1, mid_x, TOWER_TILE_FLOOR);
  world_hline(0, WORLD_CHUNK_W - 1, mid_y, TOWER_TILE_FLOOR);
  for (uint8_t r = 0; r < rooms; r++) {
    world_vline(cy[r], mid_y, cx[r], TOWER_TILE_FLOOR);
    world_hline(cx[r], mid_x, cy[r], TOWER_TILE_FLOOR);
  }
}

static void world_lay_out_blocked(void) {
  world_fill(TOWER_TILE_WALL);
}

static uint16_t world_seed_for(uint8_t cx, uint8_t cy) {
  return world_hash(cx, cy) | 1u;
}

void world_generate_chunk(uint8_t cx, uint8_t cy) BANKED {
  if (cx >= WORLD_W) cx = WORLD_W - 1;
  if (cy >= WORLD_H) cy = WORLD_H - 1;

  world_chunk_x = cx;
  world_chunk_y = cy;
  world_seed = world_seed_for(cx, cy);
  current_chunk_kind = world_kind_at(cx, cy);
  const WorldLandmark *landmark = world_landmark_at(cx, cy);
  if (landmark) {
    switch (landmark->type) {
    case LANDMARK_CASTLE:
    case LANDMARK_CITY: current_chunk_kind = CHUNK_CITY; break;
    case LANDMARK_TOWN:
    case LANDMARK_VILLAGE: current_chunk_kind = CHUNK_TOWN; break;
    case LANDMARK_DUNGEON:
    case LANDMARK_TOWER:
    case LANDMARK_BOSS_ARENA:
    case LANDMARK_RUINS:
    case LANDMARK_CAVE:
    case LANDMARK_TREASURE_VAULT: current_chunk_kind = CHUNK_DUNGEON; break;
    default: break;
    }
  }

  switch (current_chunk_kind) {
  case CHUNK_CITY: world_lay_out_settlement(9); break;
  case CHUNK_TOWN: world_lay_out_settlement(6); break;
  case CHUNK_VILLAGE: world_lay_out_settlement(3); break;
  case CHUNK_DUNGEON: world_lay_out_dungeon(); break;
  case CHUNK_OCEAN:
  case CHUNK_MOUNTAIN: world_lay_out_blocked(); break;
  default: world_lay_out_wilderness(); break;
  }
}

Map *world_map(void) BANKED { return &world_active_map; }

void world_enter(void) BANKED {
  in_world = true;
  world_generate_chunk(WORLD_W / 2, WORLD_H / 2);
}

bool world_travel(int8_t dx, int8_t dy) BANKED {
  int16_t nx = (int16_t)world_chunk_x + dx;
  int16_t ny = (int16_t)world_chunk_y + dy;
  if (nx < 0 || ny < 0 || nx >= WORLD_W || ny >= WORLD_H) return false;

  ChunkKind kind = world_kind_at((uint8_t)nx, (uint8_t)ny);
  if (kind == CHUNK_OCEAN || kind == CHUNK_MOUNTAIN) return false;

  world_generate_chunk((uint8_t)nx, (uint8_t)ny);
  const int8_t mid = WORLD_CHUNK_W / 2;
  if (dx > 0) set_hero_position(1, mid);
  else if (dx < 0) set_hero_position(WORLD_CHUNK_W - 2, mid);
  else if (dy > 0) set_hero_position(mid, 1);
  else if (dy < 0) set_hero_position(mid, WORLD_CHUNK_H - 2);
  return true;
}

bool world_on_move(void) BANKED {
  const int8_t x = hero_x();
  const int8_t y = hero_y();
  const int8_t max_x = WORLD_CHUNK_W - 1;
  const int8_t max_y = WORLD_CHUNK_H - 1;
  if (x <= 0) return world_travel(-1, 0);
  if (x >= max_x) return world_travel(1, 0);
  if (y <= 0) return world_travel(0, -1);
  if (y >= max_y) return world_travel(0, 1);
  return false;
}
