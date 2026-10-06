# Labyrinth of the Dragon — World Content

## Content hierarchy

World -> Realm Zone -> Region -> Landmark -> Route -> Encounter / Quest

The overworld uses a 16x16 grid of 32x32-tile chunks. Only the active chunk is generated, keeping RAM use bounded on Game Boy Color.

## Realm progression

| Realm | Levels | Role |
|---|---:|---|
| Crown Realm | 1-8 | Starting kingdom |
| Green Realm | 4-18 | Forest frontier |
| Sun Realm | 12-30 | Desert and ruins |
| Iron Realm | 18-42 | Mountain kingdom |
| Mist Realm | 28-55 | Undead marsh |
| Frost Realm | 40-70 | Frozen north |
| Shadow Realm | 50-80 | Corrupted ruins |
| Dragon Realm | 55-99 | Endgame dragon territory |
| Astral Realm | 65-99 | Secret endgame |
| Void Realm | 80-99 | Final forbidden zone |

## Landmark progression

Crown Castle (8,8), Greenvale Village (6,6), Sunscar Town (11,11), Ironridge Keep (11,5), Mistmarsh Shrine (4,11), Frostfang Citadel (5,3), Shadow Ruins (13,6), Dragon Waste Gate (13,13), Ancient Dragon Tower (12,4), Dragon Throne (14,14), Astral Gate (2,2), Void Gate (2,13).

## Encounter bands

Crown: Kobold / Goblin / Bugbear.
Green: Goblin / Bugbear / Owlbear.
Sun: Bugbear / Gelatinous Cube / Displacer Beast.
Iron: Owlbear / Displacer Beast / Will-o-Wisp.
Mist: Zombie / Will-o-Wisp / Death Knight.
Frost: Will-o-Wisp / Mind Flayer / Beholder.
Shadow: Death Knight / Mind Flayer / Beholder.
Dragon: Beholder / Death Knight / Dragon.
Astral: Mind Flayer / Beholder / Dragon.
Void: Death Knight / Beholder / Dragon.

## Main quest chain

1. Crown Call
2. Greenvale Goblins
3. Sunscar Vault
4. Ironridge Defense
5. Mist Shrine
6. Frostfang Citadel
7. Shadow Ruins
8. Dragon Gate
9. Ancient Dragon
10. Astral Gate
11. Void Throne

Quest IDs are stable data keys so NPC dialogue, map callbacks and save flags can reference the same progression.

## Design goals

- Towns and castles are safe hubs.
- Roads connect major landmarks.
- Dungeons provide nonlinear exploration.
- Realm level bands create a readable difficulty curve.
- Astral and Void content is optional/late-game.
- World generation remains deterministic and memory efficient.
