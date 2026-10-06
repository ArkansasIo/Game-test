#ifndef _WORLD_LANDMARKS_H
#define _WORLD_LANDMARKS_H
#include <stdint.h>
#include <stdbool.h>
#include "world.h"
#include "realm_zones.h"

typedef enum WorldLandmarkType { LANDMARK_NONE, LANDMARK_CASTLE, LANDMARK_CITY, LANDMARK_TOWN, LANDMARK_VILLAGE, LANDMARK_SHRINE, LANDMARK_CAVE, LANDMARK_TOWER, LANDMARK_DUNGEON, LANDMARK_RUINS, LANDMARK_BOSS_ARENA, LANDMARK_GATE, LANDMARK_PORTAL, LANDMARK_TREASURE_VAULT } WorldLandmarkType;
typedef struct WorldLandmark { uint8_t id; const char *name; WorldLandmarkType type; uint8_t chunk_x; uint8_t chunk_y; RealmZoneId realm; uint8_t min_level; uint8_t flags; } WorldLandmark;
#define LANDMARK_FLAG_STORY 0x01
#define LANDMARK_FLAG_SECRET 0x02
#define LANDMARK_FLAG_BOSS 0x04
#define LANDMARK_FLAG_COMPLETE 0x08
#define WORLD_LANDMARK_COUNT 16
extern const WorldLandmark world_landmarks[WORLD_LANDMARK_COUNT];
const WorldLandmark *world_landmark_at(uint8_t cx, uint8_t cy) BANKED;
const WorldLandmark *world_landmark_by_id(uint8_t id) BANKED;
bool world_landmark_unlocked(uint8_t id, uint8_t player_level) BANKED;
#endif
