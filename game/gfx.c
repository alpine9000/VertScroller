#include "game.h"

#define printf(...)
#define dprintf(...)

static uint16_t dyOffsetsLUT[FRAME_BUFFER_HEIGHT];

void 
gfx_init()
{
  for (uint16_t y = 0; y < FRAME_BUFFER_HEIGHT; y++) {
    dyOffsetsLUT[y] = (y * (SCREEN_WIDTH_BYTES*SCREEN_BIT_DEPTH));
  }
}


void
gfx_fillRect(frame_buffer_t fb, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  static volatile struct Custom* _custom = CUSTOM;
  static uint16_t startBitPatterns[] = { 0xffff,
			       0x7fff, 0x3fff, 0x1fff, 0x0fff, 
			       0x07ff, 0x03ff, 0x01ff, 0x00ff,
			       0x007f, 0x003f, 0x001f, 0x000f,
			       0x0007, 0x0003, 0x0001, 0x0000 };

  static uint16_t endBitPatterns[] = { 0xffff, 
				    0x8000, 0xc000, 0xe000, 0xf000,
				    0xf800, 0xfc00, 0xfe00, 0xff00,
				    0xff80, 0xffc0, 0xffe0, 0xfff0,
				    0xfff8, 0xfffc, 0xfffe, 0xffff};

  uint16_t startMask = startBitPatterns[x & 0xf]; 
  uint16_t endMask = endBitPatterns[(x+w) & 0xf]; 
  uint32_t widthWords = (((x&0x0f)+w)+15)>>4;
  
  if (widthWords == 1) {
    startMask &= endMask;
  }
  
  fb += dyOffsetsLUT[y] + (x>>3);

  int colorInPlane;
  for (int plane = 0; plane < SCREEN_BIT_DEPTH; plane++) {
    colorInPlane = (1<<plane) & color;
    hw_waitBlitter();
    
    _custom->bltcon0 = (SRCC|DEST|0xca);
    _custom->bltcon1 = 0;
    _custom->bltafwm = 0xffff;
    _custom->bltalwm = 0xffff;
    _custom->bltdmod = (SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(SCREEN_WIDTH_BYTES-2);
    _custom->bltcmod = (SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(SCREEN_WIDTH_BYTES-2);
    _custom->bltbmod = 0;
    _custom->bltamod = 0;
    _custom->bltadat = startMask;
    _custom->bltbdat = colorInPlane ? 0xffff : 0x0;
    _custom->bltcpt = fb;
    _custom->bltdpt = fb;
    _custom->bltsize = h<<6 | 1;
    
    if (widthWords > 1) {
      hw_waitBlitter();    
      _custom->bltcon0 = (SRCC|DEST|0xca);
      _custom->bltadat = endMask;
      _custom->bltcpt = fb+((widthWords-1)<<1);
      _custom->bltdpt = fb+((widthWords-1)<<1);
      _custom->bltsize = h<<6 | 1;
    }
    
    if (widthWords > 2) {
      hw_waitBlitter();    
      _custom->bltcon0 = (DEST|(colorInPlane ? 0xff : 0x00));
      _custom->bltdmod = (SCREEN_WIDTH_BYTES*(SCREEN_BIT_DEPTH-1))+(SCREEN_WIDTH_BYTES-((widthWords-2)<<1));
      _custom->bltdpt = fb+2;
      _custom->bltsize = h<<6 | widthWords-2;
    }    

    fb += SCREEN_WIDTH_BYTES;
  }
}

void
gfx_renderSprite(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy, int16_t w, int16_t h)
{
  static volatile struct Custom* _custom = CUSTOM;
  frame_buffer_t source = spriteFrameBuffer;
  frame_buffer_t mask = spriteMask;
  uint32_t widthWords =  ((w+15)>>4)+1;
  int shift = (dx&0xf);
  
  dest += dyOffsetsLUT[dy] + (dx>>3);
  source += dyOffsetsLUT[sy] + (sx>>3);
  mask += dyOffsetsLUT[sy] + (sx>>3);

  hw_waitBlitter();

  _custom->bltcon0 = (SRCA|SRCB|SRCC|DEST|0xca|shift<<ASHIFTSHIFT);
  _custom->bltcon1 = shift<<BSHIFTSHIFT;
  _custom->bltafwm = 0xffff;
  _custom->bltalwm = 0x0000;
  _custom->bltamod = (SCREEN_WIDTH_BYTES-(widthWords<<1));
  _custom->bltbmod = (SCREEN_WIDTH_BYTES-(widthWords<<1));
  _custom->bltcmod = (SCREEN_WIDTH_BYTES-(widthWords<<1));
  _custom->bltdmod = (SCREEN_WIDTH_BYTES-(widthWords<<1));
  _custom->bltapt = mask;
  _custom->bltbpt = source;
  _custom->bltcpt = dest;
  _custom->bltdpt = dest;
  _custom->bltsize = (h*SCREEN_BIT_DEPTH)<<6 | widthWords;
}

void
gfx_renderTile(frame_buffer_t dest, int16_t sx, int16_t sy, int16_t dx, int16_t dy)
{
  static volatile struct Custom* _custom = CUSTOM;
  frame_buffer_t source = spriteFrameBuffer;
  
  dest += dyOffsetsLUT[dy] + (dx>>3);
  source += dyOffsetsLUT[sy] + (sx>>3);

  hw_waitBlitter();

  _custom->bltcon0 = (SRCA|DEST|0xf0);
  _custom->bltcon1 = 0;
  _custom->bltafwm = 0xffff;
  _custom->bltalwm = 0xffff;
  _custom->bltamod = SCREEN_WIDTH_BYTES-2;
  _custom->bltdmod = SCREEN_WIDTH_BYTES-2;
  _custom->bltapt = source;
  _custom->bltdpt = dest;
  _custom->bltsize = (16*SCREEN_BIT_DEPTH)<<6 | 1;
}


void
gfx_renderTile2(frame_buffer_t dest, int16_t x, int16_t y, frame_buffer_t tile)
{
  static volatile struct Custom* _custom = CUSTOM;
  
  dest += dyOffsetsLUT[y] + (x>>3);

  hw_waitBlitter();

  _custom->bltcon0 = (SRCA|DEST|0xf0);
  _custom->bltcon1 = 0;
  _custom->bltafwm = 0xffff;
  _custom->bltalwm = 0xffff;
  _custom->bltamod = SCREEN_WIDTH_BYTES-2;
  _custom->bltdmod = SCREEN_WIDTH_BYTES-2;
  _custom->bltapt = tile;
  _custom->bltdpt = dest;
  _custom->bltsize = (16*SCREEN_BIT_DEPTH)<<6 | 1;
}

