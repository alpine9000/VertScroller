#ifndef ITEM_H
#define ITEM_H

#define ITEM_HEIGHT 11
#define ITEM_WIDTH 16

void
item_init(void);
void
item_saveBackground(frame_buffer_t fb);
void
item_restoreBackground(void);
void
item_render(frame_buffer_t fb);
void
item_update(sprite_t* p);
void
item_addCoin(uint32_t x, uint32_t y, unsigned short* tilePtr);
#endif