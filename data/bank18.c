/**
 * [Data] Bank 18 - Dragon Warrior-style overworld graphics
 */
#pragma bank 18

#include <gb/gb.h>
#include <gbdk/incbin.h>

/*
 * First 128 8x8 tiles from the master pixel atlas, converted to GBC 2bpp.
 * The source PNG remains the editable master artwork.
 */
INCBIN(tile_world_atlas, "res/tiles/world_atlas.bin")
