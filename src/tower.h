/**
 * Procedural tower generator.
 *
 * The tower is 100 floors assembled at runtime rather than hand-authored. Each
 * floor is a room-and-corridor layout written into a RAM buffer that the map
 * system reads through a generated `Map`.
 *
 * Because the buffer is regenerated on every floor transition, a single 32x32
 * map buffer covers all 100 floors, so the tower costs a fixed amount of RAM
 * instead of 100 tilesets' worth of ROM.
 *
 * Floor layout is seeded from the floor number, so a given floor always looks
 * the same when revisited.
 */
#ifndef _TOWER_H
#define _TOWER_H

#include "biome.h"
#include "core.h"
#include "map.h"

/**
 * Total number of floors in the tower.
 */
#define TOWER_FLOORS 100

/**
 * Width and height of a generated floor, in map tiles.
 */
#define TOWER_MAP_W 32
#define TOWER_MAP_H 32

/**
 * Number of rooms placed per floor.
 */
#define TOWER_ROOMS 5

/**
 * Map data byte for a solid wall.
 */
#define TOWER_TILE_WALL 0x00

/**
 * Map data byte for a walkable floor tile.
 */
#define TOWER_TILE_FLOOR 0x41

/**
 * Map data byte for a special tile. Special tiles are dispatched to the floor's
 * `on_special` callback, which is how the tower's stairs work: the stair
 * position is only known at runtime, so a static `Exit` table cannot describe
 * it.
 */
#define TOWER_TILE_SPECIAL 0xC1

/**
 * Which floor of the tower the player is currently on, 1-based.
 */
extern uint8_t tower_floor;

/**
 * Whether the player is currently inside the tower.
 */
extern bool in_tower;

/**
 * Enters the tower at floor 1.
 */
void tower_enter(void) BANKED;

/**
 * Leaves the tower, returning to the main dungeon.
 */
void tower_leave(void) BANKED;

/**
 * Generates a floor into the shared map buffer and makes it the active map.
 * @param floor Floor number to generate, 1..TOWER_FLOORS.
 */
void tower_generate_floor(uint8_t floor) BANKED;

/**
 * Moves up one floor, if not already at the top.
 * @return `true` if the player moved.
 */
bool tower_ascend(void) BANKED;

/**
 * Moves down one floor, if not already at the bottom.
 * @return `true` if the player moved.
 */
bool tower_descend(void) BANKED;

/**
 * @return The biome that floor of the tower uses.
 */
BiomeId tower_biome_for_floor(uint8_t floor) BANKED;

/**
 * Floor callback invoked when the player steps onto a special tile. Handles the
 * up and down stairs. Intended to be assigned to `Floor.on_special`.
 * @return `true` if the tile was handled.
 */
bool tower_on_special(void) BANKED;

/**
 * @return Column of the up stairs on the current floor, or 0 if none.
 */
uint8_t tower_up_stairs_x(void) BANKED;

/**
 * @return Row of the up stairs on the current floor, or 0 if none.
 */
uint8_t tower_up_stairs_y(void) BANKED;

/**
 * @return Column of the down stairs on the current floor, or 0 if none.
 */
uint8_t tower_down_stairs_x(void) BANKED;

/**
 * @return Row of the down stairs on the current floor, or 0 if none.
 */
uint8_t tower_down_stairs_y(void) BANKED;

/**
 * @return The map for the currently generated tower floor.
 */
Map *tower_map(void) BANKED;

#endif
