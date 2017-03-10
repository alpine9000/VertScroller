#include "game.h"

int
strlen(char* s) 
{
  int count = 0;
  while (*s++ != 0) {
    count++;
  }

  return count;
}


char *
strcat(char *dest, const char *src)
{
  strcpy(&dest[strlen(dest)], src);
  return dest;
}


char *
itoa(int32_t i)
{
  static char buf[12];
  char *p = buf + 11;
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  } else {
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}



char *
strcpy(char *dest, const char *src)
{
  char *s = dest;
  while ((*s++ = *src++) != 0);
  return dest;
}

static char _text_hex[] = {'A', 'B', 'C', 'D', 'E', 'F'};
static char _text_buf[9];

static inline 
int _hexChar(int s)
{
  int c;

  c = (s >= 0 && s <= 9) ? s + '0' : _text_hex[s - 10];
  return c;
}


char*
itoh(uint32_t n, uint16_t numChars)
{
  uint32_t c;
  char* ptr = &_text_buf[numChars];
  *ptr = 0;
  ptr--;
  for (c = 1; c <= numChars; c++) {
    *ptr = _hexChar(n & 0xf);
    ptr--;
    n = n >> 4;
  }

  return _text_buf;
}
