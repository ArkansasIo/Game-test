# Labyrinth of the Dragon — Overworld

The overworld is a 16 x 16 chunk world. Each chunk is 32 x 32 tiles and is
generated deterministically, so the cartridge stores only the active chunk in RAM.

## World regions

| Region | Approx. area | Level band | Role |
|---|---|---:|---|
| Crownlands | center / inner ring | 1–8 | Starting kingdom and early quests |
| Greenvale | northwest | 4–18 | Forests, farms, villages and first caves |
| Sunscar | southeast interior | 12–30 | Dry plains, ruins and fire-themed enemies |
| Ironridge | central-east | 18–42 | Mountain passes, mines and fortress towns |
| Mistmarsh | southwest | 28–55 | Swamps, crypts and cursed settlements |
| Frostfang | north/east | 40–70 | Snow passes, ice caves and late-game trials |
| Dragon Waste | outer interior | 55–99 | Volcanic wasteland and dragon territory |
| Deep Ocean | outer border | — | Impassable boundary until sea travel exists |

## Major landmarks

- Crown Castle — starting capital and story hub
- Greenvale Village — first village
- Ironridge Keep — fortress / mid-game hub
- Mistmarsh Shrine — curse-cleansing story location
- Frostfang Citadel — late-game kingdom
- Dragon Waste Gate — final-region checkpoint
- Ancient Dragon Tower — final dungeon
- Dragon Throne — final boss arena

## Traversal

1. The center chunk is always the starting city.
2. Adjacent inner-ring chunks are towns.
3. Outer chunks are deterministic wilderness, villages, dungeons or mountains.
4. Ocean and mountain chunks are blocked.
5. Leaving a traversable chunk generates the adjacent chunk and places the hero at
   the opposite edge.
6. Regions define geographic encounter progression.

## Encounter progression

Crownlands -> Greenvale -> Sunscar -> Ironridge -> Mistmarsh -> Frostfang -> Dragon Waste

The existing biome system supplies monster rosters; this region layer supplies
geographic progression without storing 256 complete maps.
