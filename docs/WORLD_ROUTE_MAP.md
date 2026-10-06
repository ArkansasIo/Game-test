# Dragon Warrior World Route Map

```text
                         [Astral Gate]
                              |
                              |
 [Frostfang Citadel] --- [Ironridge Keep]
        |                       |
        |                       |
 [Mistmarsh Shrine] --- [Crown Castle] --- [Greenvale Village]
                                      \\\             |
                                       \\\            |
                                        \\\      [Sunscar Town]
                                         \\\           |
                                          \\\          |
                                      [Dragon Waste Gate]
                                             |
                                     [Ancient Dragon Tower]
                                             |
                                      [Dragon Throne]

 Secret branch: [Shadow Ruins]
```

The world center is chunk (8,8). Landmark coordinates are defined in `src/world_landmarks.c`; route data is defined in `src/world_routes.c`.

## Travel rules

- Ocean and mountain chunks are impassable.
- Landmark chunks override normal procedural chunk generation.
- Realm and landmark level requirements are data-driven.
- `world_route_allowed()` exposes route level requirements.
- Route detection currently uses a coarse bounding-box test; future terrain generation should carve narrow visible road corridors that match the logical routes exactly.

## Integration points

1. Connect `world_route_allowed()` to edge travel when the player-level API is available.
2. Use `world_landmark_unlocked()` for landmark entry.
3. Feed `world_zone_level()` and `world_zone_monster()` into random encounter generation.
4. Connect `world_quest_*()` to NPC callbacks and persistent quest flags.
