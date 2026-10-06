#pragma bank 7
#include "world_routes.h"

const WorldRoute world_routes[WORLD_ROUTE_COUNT] = {
{0,"Kingsroad",8,8,6,6,1},
{1,"Green Road",6,6,11,11,4},
{2,"Iron Road",11,11,11,5,12},
{3,"North Road",11,5,5,3,18},
{4,"Marsh Road",6,6,4,11,8},
{5,"Frost Road",4,11,5,3,28},
{6,"Dragon Road",5,3,13,13,40},
{7,"Tower Road",13,13,12,4,55},
{8,"Shadow Road",12,4,13,6,50},
{9,"Astral Road",8,8,2,2,65},
{10,"Dragon Throne Road",13,13,14,14,70},
{11,"Void Road",13,13,2,13,80}
};

static uint8_t near(uint8_t a, uint8_t b) {
  return a > b ? a - b : b - a;
}

static bool corridor_segment(uint8_t cx, uint8_t cy, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  uint8_t lx = x1 < x2 ? x1 : x2;
  uint8_t hx = x1 > x2 ? x1 : x2;
  uint8_t ly = y1 < y2 ? y1 : y2;
  uint8_t hy = y1 > y2 ? y1 : y2;
  if (cx >= lx && cx <= hx && near(cy, y1) <= 1) return true;
  if (cy >= ly && cy <= hy && near(cx, x2) <= 1) return true;
  return false;
}

bool world_route_corridor(uint8_t cx,uint8_t cy) BANKED {
  uint8_t i;
  for (i = 0; i < WORLD_ROUTE_COUNT; i++) {
    const WorldRoute *r = &world_routes[i];
    if (corridor_segment(cx, cy, r->from_x, r->from_y, r->to_x, r->to_y))
      return true;
  }
  return false;
}

uint8_t world_route_between(uint8_t cx,uint8_t cy) BANKED {
  uint8_t i;
  for (i = 0; i < WORLD_ROUTE_COUNT; i++) {
    const WorldRoute *r = &world_routes[i];
    if (corridor_segment(cx, cy, r->from_x, r->from_y, r->to_x, r->to_y))
      return r->id;
  }
  return ROUTE_NONE;
}

bool world_route_allowed(uint8_t cx,uint8_t cy,uint8_t level) BANKED {
  uint8_t r = world_route_between(cx,cy);
  return r == ROUTE_NONE || level >= world_routes[r].min_level;
}
