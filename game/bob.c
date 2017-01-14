#include "game.h"

typedef struct {
  int16_t x;
  int16_t y;
  int16_t w;
  int16_t h;
} bob_t;


static
bob_t bobs[] = {
  [BOB_CLIMBER_RUN_LEFT_1] = { // 0. climber run left 1
    .x = 0,
    .y = 80,
    .w = 32,
    .h = 48
  },
  [BOB_CLIMBER_RUN_LEFT_2] = { // 1. climber run left 2
    .x = 32,
    .y = 80,
    .w = 32,
    .h = 48
  },
  [BOB_CLIMBER_RUN_LEFT_3] = { // 2. climber run left 3
    .x = 64,
    .y = 80,
    .w = 32,
    .h = 48
  },
  [BOB_CLIMBER_RUN_LEFT_4] = { // 3. climber run left 4
    .x = 96,
    .y = 80,
    .w = 32,
    .h = 48
  },
  [BOB_CLIMBER_STAND_LEFT] =  { // 4. climber stand left 
    .x = 128,
    .y = 80,
    .w = 32,
    .h = 48
  },
  [BOB_CLIMBER_JUMP_LEFT] =  { // 5. climber jump left
    .x = 160,
    .y = 80,
    .w = 32,
    .h = 48
  },
  [BOB_CLIMBER_RUN_RIGHT_1] = { // 6. climber run right 1
    .x = 0,
    .y = 128,
    .w = 32,
    .h = 48
  },
  [BOB_CLIMBER_RUN_RIGHT_2] = { // 7. climber run right 2
    .x = 32,
    .y = 128,
    .w = 32,
    .h = 48
  },
  [BOB_CLIMBER_RUN_RIGHT_3] = { // 8. climber run right 3
    .x = 64,
    .y = 128,
    .w = 32,
    .h = 48
  },
  [BOB_CLIMBER_RUN_RIGHT_4] = { // 9. climber run right 4
    .x = 96,
    .y = 128,
    .w = 32,
    .h = 48
  },
  [BOB_CLIMBER_STAND_RIGHT] = { // 10. climber stand right 
    .x = 128,
    .y = 128,
    .w = 32,
    .h = 48
  },
  [BOB_CLIMBER_JUMP_RIGHT]  = { // 11. climber jump right
    .x = 160,
    .y = 128,
    .w = 32,
    .h = 48
  },
  [BOB_CLOUD_1]  = {
    .x = 0,
    .y = 32,
    .w = 64,
    .h = 32
  },
};

void
bob_save(frame_buffer_t fb, int16_t x, int16_t y, uint16_t b, bob_save_t* save)
{
  bob_t* bob = &bobs[b];
  int h = bob->h;
  if (y < cameraY) {
    h -= (cameraY - y);
    y += (cameraY - y);
    if (h <= 0) {
      save->blit[0].size = 0;
      save->blit[1].size = 0;
      return;
    }
  }
  y = y-cameraY-screenScrollY;
  if (y >= 0) {
    gfx_saveSprite(fb, &save->blit[0], x, y, bob->w, h);
    save->blit[1].size = 0;
  } else {
    if (y > -h) {
      gfx_saveSprite(fb, &save->blit[0], x, 0, bob->w, h+y);    
      gfx_saveSprite(fb, &save->blit[1], x, FRAME_BUFFER_HEIGHT+y, bob->w, -y);    
    } else {
      gfx_saveSprite(fb, &save->blit[0], x, FRAME_BUFFER_HEIGHT+y, bob->w, h);    
      save->blit[1].size = 0;
    }
  }

}



/*

  y = 10
  cameraY = 20

*/

volatile int16_t d_by, d_sy, d_cy;
void
bob_render(frame_buffer_t fb, int16_t x, int16_t y, uint16_t b)
{
  bob_t* bob = &bobs[b];
  int by = bob->y;
  int h = bob->h;
  if (y < cameraY) {
    h -= (cameraY - y);
    by += (cameraY - y);
    y += (cameraY - y);
    if (h <= 0) {
      return;
    }
  }


    
  y = y-cameraY-screenScrollY;
  if (y >= 0) {
    gfx_renderSprite(fb, bob->x, by, x, y, bob->w, h);
  } else {
    if (y > -h) {
      gfx_renderSprite(fb, bob->x, by-y, x, 0, bob->w, h+y);    
      gfx_renderSprite(fb, bob->x, by, x, FRAME_BUFFER_HEIGHT+y, bob->w, -y);    
    } else {
      gfx_renderSprite(fb, bob->x, by, x, FRAME_BUFFER_HEIGHT+y, bob->w, h);    
    }
  }
}


void
bob_clear(bob_save_t* save)
{
  if (save->blit[0].size > 0) {
    gfx_clearSprite(&save->blit[0]);
  }

  if (save->blit[1].size > 0) {
    gfx_clearSprite(&save->blit[1]);
  }
}
