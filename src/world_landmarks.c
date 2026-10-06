#pragma bank 7
#include "world_landmarks.h"

const WorldLandmark world_landmarks[WORLD_LANDMARK_COUNT] = {
{0,"Crown Castle",LANDMARK_CASTLE,8,8,REALM_ZONE_CROWN_REALM,1,LANDMARK_FLAG_STORY},
{1,"Greenvale Village",LANDMARK_VILLAGE,6,6,REALM_ZONE_GREEN_REALM,4,0},
{2,"Sunscar Town",LANDMARK_TOWN,11,11,REALM_ZONE_SUN_REALM,12,0},
{3,"Ironridge Keep",LANDMARK_CASTLE,11,5,REALM_ZONE_IRON_REALM,18,LANDMARK_FLAG_STORY},
{4,"Mistmarsh Shrine",LANDMARK_SHRINE,4,11,REALM_ZONE_MIST_REALM,28,LANDMARK_FLAG_STORY},
{5,"Frostfang Citadel",LANDMARK_CASTLE,5,3,REALM_ZONE_FROST_REALM,40,LANDMARK_FLAG_STORY},
{6,"Shadow Ruins",LANDMARK_RUINS,13,6,REALM_ZONE_SHADOW_REALM,50,LANDMARK_FLAG_SECRET},
{7,"Dragon Waste Gate",LANDMARK_GATE,13,13,REALM_ZONE_DRAGON_REALM,55,LANDMARK_FLAG_STORY},
{8,"Ancient Dragon Tower",LANDMARK_TOWER,12,4,REALM_ZONE_DRAGON_REALM,60,LANDMARK_FLAG_STORY},
{9,"Dragon Throne",LANDMARK_BOSS_ARENA,14,14,REALM_ZONE_DRAGON_REALM,70,LANDMARK_FLAG_BOSS},
{10,"Astral Gate",LANDMARK_PORTAL,2,2,REALM_ZONE_ASTRAL_REALM,65,LANDMARK_FLAG_SECRET},
{11,"Void Gate",LANDMARK_PORTAL,2,13,REALM_ZONE_VOID_REALM,80,LANDMARK_FLAG_BOSS|LANDMARK_FLAG_SECRET},
{12,"Kingsroad Shrine",LANDMARK_SHRINE,8,5,REALM_ZONE_CROWN_REALM,1,0},
{13,"Moonlit Cave",LANDMARK_CAVE,7,10,REALM_ZONE_GREEN_REALM,8,LANDMARK_FLAG_SECRET},
{14,"Sunken Vault",LANDMARK_TREASURE_VAULT,10,12,REALM_ZONE_SUN_REALM,16,LANDMARK_FLAG_SECRET},
{15,"Demonwatch Ruins",LANDMARK_DUNGEON,12,7,REALM_ZONE_SHADOW_REALM,55,LANDMARK_FLAG_BOSS}
};

const WorldLandmark *world_landmark_at(uint8_t cx,uint8_t cy) BANKED { uint8_t i; for(i=0;i<WORLD_LANDMARK_COUNT;i++) if(world_landmarks[i].chunk_x==cx && world_landmarks[i].chunk_y==cy) return &world_landmarks[i]; return 0; }
const WorldLandmark *world_landmark_by_id(uint8_t id) BANKED { return id<WORLD_LANDMARK_COUNT ? &world_landmarks[id] : 0; }
bool world_landmark_unlocked(uint8_t id,uint8_t player_level) BANKED { const WorldLandmark *l=world_landmark_by_id(id); return l && player_level>=l->min_level; }
