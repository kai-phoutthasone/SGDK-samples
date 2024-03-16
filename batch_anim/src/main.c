#include <genesis.h>
#include <resources.h>

#define MAX_BULLETS (80)

Sprite *bullets[MAX_BULLETS];
s16 xs[MAX_BULLETS];
s16 ys[MAX_BULLETS];

u16 vram_ind;

u16 **bullet_vram;

void bullet_update(Sprite *master) {
    u16 tileIndex = bullet_vram[master->animInd][master->frameInd];

    for (u8 i = 0; i < MAX_BULLETS; ++i) {
        Sprite *spr = bullets[i];
        SPR_setVRAMTileIndex(spr, tileIndex);
    }
}

int main() {
    SPR_init();

    // Keep track of vram index when allocating vram for the tiles they all
    // share. You can see similar functionality in 'sonic' sample.
    vram_ind = TILE_USER_INDEX;

    bullets[0] =
        SPR_addSprite(&bullet, 10, 0, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    SPR_setAutoTileUpload(bullets[0], FALSE);
    SPR_setFrameChangeCallback(bullets[0], bullet_update);

    SPR_setDepth(bullets[0], SPR_MIN_DEPTH);

    xs[0] = 10;
    ys[0] = 0;

    u16 num_tiles;
    bullet_vram =
        SPR_loadAllFrames(bullets[0]->definition, vram_ind, &num_tiles);

    for (u16 i = 1; i < MAX_BULLETS; ++i) {
        u16 x = 10 + (i / 20) * 16;
        u16 y = (i % 20) * 8;

        bullets[i] = SPR_addSpriteEx(bullets[0]->definition, x, y,
                                     TILE_ATTR(PAL1, TRUE, FALSE, FALSE), 0);

        xs[i] = x;
        ys[i] = y;
    }

    // Incase we want to load anything afterwards, we increment
    vram_ind += num_tiles;

    u16 tick = 0;

    while (1) {
        SPR_update();
        SYS_doVBlankProcess();
        if (tick++ == 10) {
            tick = 0;

            for (u16 i = 0; i < MAX_BULLETS; ++i) {
                xs[i]++;
                SPR_setPosition(bullets[i], xs[i], ys[i]);
            }
        }
    }
    return (0);
}