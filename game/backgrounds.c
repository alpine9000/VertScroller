#include "game.h"
unsigned short background_tileAddresses[202][16];
#if defined(__GNUC__)
__section(section noload)
#else
__section(noload)
#endif
#include "out/background-map.c"
