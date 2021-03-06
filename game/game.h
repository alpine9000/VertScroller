#ifndef __GAME_H
#define __GAME_H

#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <hardware/intbits.h>

#define DEBUG                         1
//#define ASSERT_CHECK                  1
#define INLINE_EVERYTHING             1
#define GAME_JETPACK                  1
#define GAME_KEYBOARD_ENABLED         1
#define GAME_RECORDING                1
//#define GAME_BLITTER_WAIT_DEBUG       1
#define PLAYER_HARDWARE_SPRITE        1
//#define CLOUD_FULLCOLOR             1

#ifdef INLINE_EVERYTHING
#define INLINE static inline
#define STATIC_INLINE static inline
#else
#define INLINE
#define STATIC_INLINE static
#endif

#define abs(a) ((a) >= 0 ? (a) : -(a))  
#define max(a, b) ((a) > (b) ? (a) : (b))

#define MENU_NUM_ITEMS      6

#define MAP_TILE_WIDTH      16
#define MAP_TILE_HEIGHT     201
#define TILE_WIDTH          16
#define TILE_HEIGHT         16

#define CUSTOM ((struct Custom*)0xdff000)
#define SCREEN_WIDTH        MAP_TILE_WIDTH*TILE_WIDTH
#define SCREEN_HEIGHT       256
#define FRAME_BUFFER_OFFSCREEN_HEIGHT TILE_HEIGHT*4
#define FRAME_BUFFER_NUM_LINES ((SCREEN_HEIGHT+FRAME_BUFFER_OFFSCREEN_HEIGHT)+1)
#define FRAME_BUFFER_MAX_HEIGHT (FRAME_BUFFER_NUM_LINES-1)
#define FRAME_BUFFER_WIDTH  (SCREEN_WIDTH+64)

#define SCREEN_HEIGHT_WORDS SCREEN_HEIGHT/16
#define SCREEN_WIDTH_BYTES  (SCREEN_WIDTH/8)
#define FRAME_BUFFER_WIDTH_BYTES  (FRAME_BUFFER_WIDTH/8)
#define SCREEN_WIDTH_WORDS  (SCREEN_WIDTH/16)
#define SCREEN_BIT_DEPTH    5
#define SCREEN_RES	    8 // 8=lo resolution, 4=hi resolution
#define RASTER_X_START	    (0x81+((320-SCREEN_WIDTH)/2)) /* hard coded coordinates from hardware manual */
#define RASTER_Y_START	    0x1d //0x25 0x1d = max vertical overscan 
#define RASTER_X_STOP	    RASTER_X_START+SCREEN_WIDTH
#define RASTER_Y_STOP	    RASTER_Y_START+SCREEN_HEIGHT+SCOREBOARD_HEIGHT
#define MENU_RASTER_Y_STOP RASTER_Y_START+SCREEN_HEIGHT
#define SCOREBOARD_HEIGHT   16

#define WORLD_HEIGHT        (MAP_TILE_HEIGHT*TILE_HEIGHT)
#define SCROLL_PIXELS       4

#define PHYSICS_VELOCITY_G        1
#define PHYSICS_TERMINAL_VELOCITY (SCROLL_PIXELS*2)
#define PHYSICS_VELOCITY_RUN      2
#define PHYSICS_VELOCITY_JUMP     -16
#define PHYSICS_VELOCITY_KILL     -10
#ifdef GAME_JETPACK
#define PHYSICS_VELOCITY_JETPACK  -4
#define PHYSICS_TERMINAL_JETPACK_V (SCROLL_PIXELS)
#endif

typedef UBYTE uint8_t;
typedef SHORT int16_t;
typedef USHORT uint16_t;
typedef LONG int32_t;
typedef ULONG uint32_t;
typedef ULONG size_t;

#define __NOLOAD __section(section noload)

#define __EXTERNAL __attribute__((externally_visible))

extern void* memcpy(void* destination, void* source, size_t num);
extern void* memset(void *dst, int c, size_t n);

#undef __chip
#define __section(x) __attribute__ ((section (#x))) 
#define __REG(reg, arg) register arg asm(reg)
#define USE(x) do { x = x; } while(0);

typedef volatile uint8_t* frame_buffer_t;
typedef volatile struct Custom* custom_t;

#define custom ((custom_t)0xdff000)
extern int16_t game_cameraY;
extern int16_t game_screenScrollY;
extern int16_t game_collisions;
extern frame_buffer_t game_offScreenBuffer;
extern frame_buffer_t game_onScreenBuffer;
extern frame_buffer_t game_scoreBoardFrameBuffer;
extern uint32_t game_score;
extern uint16_t game_over;
extern uint16_t game_levelComplete;

#include "string.h"
#include "registers.h"
#include "hw.h"
#include "panic.h"
#include "disk.h"
#include "init.h"
#include "screen.h"
#include "hiscore.h"
#include "menu.h"
#include "palette.h"
#include "record.h"
#include "level3.h"
#include "level.h"
#include "gfx.h"
#include "mouse.h"
#include "sprite.h"
#include "item.h"
#include "enemy.h"
#include "tile.h"
#include "copper.h"
#include "music.h"
#include "actor.h"
#include "player.h"
#include "cloud.h"
#include "text.h"
#include "sound.h"
#include "keyboard.h"
#include "message.h"
#include "popup.h"
#include "dos.h"
#include "script.h"

#define game_fire() ((!(hw_lastJoystickButton&0x1) && (hw_joystickButton&0x1)) || \
			(keyboard_key && keyboard_code == KEYBOARD_CODE_RETURN))
void 
game_loop(void);
void
game_requestCameraY(int16_t targetCameraY);
void
game_shakeScreen(void);
void
game_loseLife(void);
void
game_setLevelComplete(void);

#endif /* __GAME_H */
