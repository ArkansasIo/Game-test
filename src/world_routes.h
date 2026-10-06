#ifndef _WORLD_ROUTES_H
#define _WORLD_ROUTES_H
#include <stdint.h>
#include <stdbool.h>
#include "world.h"

typedef struct WorldRoute { uint8_t id; const char *name; uint8_t from_x,from_y,to_x,to_y,min_level; } WorldRoute;
#define WORLD_ROUTE_COUNT 12
#define ROUTE_NONE 0xFF
extern const WorldRoute world_routes[WORLD_ROUTE_COUNT];
bool world_route_corridor(uint8_t cx,uint8_t cy) BANKED;
uint8_t world_route_between(uint8_t cx,uint8_t cy) BANKED;
bool world_route_allowed(uint8_t cx,uint8_t cy,uint8_t player_level) BANKED;
#endif
