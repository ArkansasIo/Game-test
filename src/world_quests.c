#pragma bank 7
#include "world_quests.h"
static const WorldQuest quests[WORLD_QUEST_COUNT] = {
  { 0, "Crown Call", "Answer the king's summons and begin the Dragon Road.", 1, 25, 100, WORLD_QUEST_NONE },
  { 1, "Greenvale Goblins", "Clear the goblin threat from Greenvale's roads.", 4, 75, 350, 0 },
  { 2, "Sunscar Vault", "Recover the royal relic from the buried Sunscar vault.", 12, 150, 900, 1 },
  { 3, "Ironridge Defense", "Defend Ironridge Keep from the monsters of the pass.", 18, 300, 1800, 2 },
  { 4, "Mist Shrine", "Restore the shrine and break the curse over Mistmarsh.", 28, 500, 3200, 3 },
  { 5, "Frostfang Citadel", "Reach Frostfang and secure the northern road.", 40, 800, 5500, 4 },
  { 6, "Shadow Ruins", "Investigate the corrupted ruins beyond the northern road.", 50, 1200, 8000, 5 },
  { 7, "Dragon Gate", "Open the gate into the Dragon Waste.", 55, 1800, 12000, 6 },
  { 8, "Ancient Dragon", "Defeat the guardian of the Ancient Dragon Tower.", 60, 2500, 18000, 7 },
  { 9, "Astral Gate", "Enter the hidden Astral Realm and learn the final prophecy.", 65, 4000, 25000, 8 },
  { 10, "Void Throne", "Cross the forbidden gate and defeat the final dragon sovereign.", 80, 10000, 50000, 9 }
};
const WorldQuest *world_quest_get(uint8_t id) BANKED {
  return id < WORLD_QUEST_COUNT ? &quests[id] : 0;
}
bool world_quest_level_ok(uint8_t id, uint8_t player_level) BANKED {
  const WorldQuest *q = world_quest_get(id);
  return q && player_level >= q->min_level;
}
bool world_quest_prerequisite_ok(uint8_t id, const uint8_t *completed) BANKED {
  const WorldQuest *q = world_quest_get(id);
  if (!q) return false;
  if (q->prerequisite == WORLD_QUEST_NONE) return true;
  return completed && completed[q->prerequisite] != 0;
}
