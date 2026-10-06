# Dragon World Pixel Assets

The Dragon Warrior-style overworld uses:

- Master artwork: `assets/pixel/labyrinth_of_the_dragon/dragon_world_asset_atlas.png`
- GBC converter: `tools/atlas2gbc`
- Generated ROM tile data: `res/tiles/world_atlas.bin`
- ROM bank: **18**
- Runtime symbol: `tile_world_atlas`
- Runtime loader: `core.load_world_tiles()`

## Conversion

The master atlas is 384x256, so it contains 48x32 = 1536 8x8 source tiles.

A Game Boy Color background tile is 16 bytes of 2bpp data. The build currently extracts the first 128 source tiles into a 2,048-byte ROM resource. This keeps the first world graphics page within a single ROM bank and leaves the rest of the atlas available as source artwork for additional pages.

The converter maps arbitrary atlas colors to four luminance levels. The GBC palette is supplied at runtime, allowing the same tile art to appear as Crown, Sun, Mist, Frost, Shadow, Dragon, Astral, or Void terrain.

## Runtime mapping

When `in_world` is true, the map renderer treats the low six bits of the world map tile byte as a direct atlas tile ID. Existing dungeon/floor maps continue to use `map_tile_lookup[]`.

Current procedural world tiles therefore remain compatible:

- `TOWER_TILE_WALL = 0x00` -> atlas tile 0
- `TOWER_TILE_FLOOR = 0x41` -> atlas tile 1

The upper two bits continue to be available for GBC map attributes.

## Build

```bash
npm install
make assets
make
```

`make assets` generates `res/tiles/world_atlas.bin` before the bank-18 data object is compiled.
