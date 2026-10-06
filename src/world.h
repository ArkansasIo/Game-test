/**
 * Chunked overworld.
 *
 * The world is a grid of `WORLD_W x WORLD_H` chunks. Each chunk is a single
 * 32x32 map generated procedurally from its own coordinates, so the whole world
 * is reproducible without storing it. Only the chunk the player is standing in
 * is ever held in RAM, which is what makes a contiguous world possible on a
 * Game Boy: walking off one edge generates the neighbouring chunk and places
 * the player on the opposite edge of it.
 *
 * Chunks carry a `ChunkKind`, which decides how they are laid out. Settlements
 * (cities, towns, villages) are laid out as buildings around a plaza; the
 * wilderness is open terrain with scattered features; dungeons are the same
 * room-and-corridor style used by the tower.
 */
#ifndef _WORLD_H
#define _WORLD_H

#include <stdbool.h>
#include <stdint.h>

#include "biome.h"
#include "core.h"
#include "map.h"

/**
 * Size of the world in chunks.
 */
#define WORLD_W 16
#define WORLD_H 16

/**
 * Size of a single chunk, in map tiles.
 */
#define WORLD_CHUNK_W 32
#define WORLD_CHUNK_H 32

/**
 * What kind of place a chunk is. This drives both the layout algorithm and the
 * palette the chunk is drawn with.
 */
typedef enum ChunkKind
{
  /**
   * Open wilderness. Scattered trees, rocks and water.
   */
  CHUNK_WILDERNESS,
  /**
   * A small settlement: a few houses around a well.
   */
  CHUNK_VILLAGE,
  /**
   * A town: more buildings, a wall and a gate.
   */
  CHUNK_TOWN,
  /**
   * A city: dense buildings, paved roads, a central plaza.
   */
  CHUNK_CITY,
  /**
   * A dungeon entrance sitting in the wilderness.
   */
  CHUNK_DUNGEON,
  /**
   * Deep water. Impassable, used to shape the coastline.
   */
  CHUNK_OCEAN,
  /**
   * Mountains. Impassable, used to shape the land.
   */
  CHUNK_MOUNTAIN,
} ChunkKind;

/**
 * Number of chunk kinds.
 */
#define CHUNK_KIND_COUNT 7

/**
 * Position of the player within the world, in chunks.
 */
extern uint8_t world_chunk_x;
extern uint8_t world_chunk_y;

/**
 * Whether the player is currently in the overworld.
 */
extern bool in_world;

/**
 * The kind of place the player is currently standing in.
 */
extern ChunkKind current_chunk_kind;

/**
 * Enters the overworld at its starting chunk.
 */
void world_enter(void) BANKED;

/**
 * Generates the chunk at the given world coordinates into the shared buffer
 * and makes it the active map.
 * @param cx Chunk column.
 * @param cy Chunk row.
 */
void world_generate_chunk(uint8_t cx, uint8_t cy) BANKED;

/**
 * Moves the player into the adjacent chunk in the given direction, generating
 * it and placing the player on the opposite edge.
 * @param dx Horizontal direction, -1, 0 or 1.
 * @param dy Vertical direction, -1, 0 or 1.
 * @return `true` if the player moved.
 */
bool world_travel(int8_t dx, int8_t dy) BANKED;

/**
 * @param cx Chunk column.
 * @param cy Chunk row.
 * @return The kind of place that chunk is.
 */
ChunkKind world_kind_at(uint8_t cx, uint8_t cy) BANKED;

/**
 * @param kind Chunk kind.
 * @return A human readable name for the kind of place.
 */
const char *world_kind_name(ChunkKind kind) BANKED;

/**
 * @param cx Chunk column.
 * @param cy Chunk row.
 * @return The biome that chunk uses.
 */
BiomeId world_biome_at(uint8_t cx, uint8_t cy) BANKED;

/**
 * @return The map for the currently generated chunk.
 */
Map *world_map(void) BANKED;

/**
 * Floor callback that handles walking off the edge of a chunk. Intended to be
 * assigned to `Floor.on_move`.
 * @return `true` if the tile was handled.
 */
bool world_on_move(void) BANKED;

#endif
