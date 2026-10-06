# Realm & World Zone Design

The game world is divided into connected realms rather than treating every chunk as
the same wilderness. The zone layer sits above the existing procedural chunk system.

## Realm progression

**Crown → Green → Sun → Iron → Mist → Frost → Shadow → Dragon**

The **Astral Realm** is a secret endgame route and the **Void Realm** is the forbidden
outer boundary.

| Realm | Levels | Primary content |
|---|---:|---|
| Crown | 1–8 | Castle, villages, tutorial quests, first dungeon |
| Green | 4–18 | Forests, ruins, fairy roads, caves |
| Sun | 12–30 | Desert, oases, fire ruins, pyramids |
| Iron | 18–42 | Mountains, mines, keeps, bridges |
| Mist | 28–55 | Swamps, crypts, cursed villages |
| Frost | 40–70 | Snowfields, ice caves, northern citadels |
| Shadow | 50–80 | Demon towers, cursed castles, dark forests |
| Dragon | 55–99 | Volcanoes, dragon lairs, ancient towers, final throne |
| Astral | 65–99 | Secret sky realm, celestial ruins, superbosses |
| Void | 80–99 | Endgame forbidden zone and final challenge |

## Zone mechanics

- **Safe zones:** reduced/no random encounters around settlements.
- **Danger zones:** normal or increased encounter rates.
- **Boss zones:** boss gates and scripted encounters can be attached.
- **Secret zones:** require a quest flag, key item or special traversal ability.
- **Endgame zones:** intended for level 55+ progression.
- Zone level bands feed encounter generation and should never directly modify player
  stats; encounter generation remains the authority for combat difficulty.

## Landmark classes

Every realm can contain:

- castle
- town
- village
- shrine
- inn
- weapon shop
- armor shop
- item shop
- cave
- tower
- dungeon
- ruins
- bridge
- mountain pass
- secret entrance
- boss arena
- treasure vault

This allows the same 32×32 procedural chunk format to represent a much larger
Dragon Warrior-style world without requiring a giant static overworld map.
