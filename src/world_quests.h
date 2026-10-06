#ifndef _WORLD_QUESTS_H
#define _WORLD_QUESTS_H
#include <stdbool.h>
#include <stdint.h>
#include "core.h"
#define WORLD_QUEST_NONE 0xFF
#define WORLD_QUEST_COUNT 11
typedef struct WorldQuest {
  uint8_t id;
  const char *name;
  const char *description;
  uint8_t min_level;
  uint16_t reward_gold;
  uint16_t reward_xp;
  uint8_t prerequisite;
} WorldQuest;
const WorldQuest *world_quest_get(uint8_t id) BANKED;
bool world_quest_level_ok(uint8_t id, uint8_t player_level) BANKED;
bool world_quest_prerequisite_ok(uint8_t id, const uint8_t *completed) BANKED;
#endif
