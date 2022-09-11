#ifndef _CR_TYPES_H_
#define _CR_TYPES_H_

#include <inttypes.h>

typedef struct Color {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

typedef struct Rect {
    int x;
    int y;
    int w;
    int h;
} Rect;

typedef struct ScreenBuffer {
    uint8_t* pixels;
    int width;
    int height;
} ScreenBuffer;

typedef enum TileType {
    StaticScene,
    StaticPlayer,
    FirstPerson
} TileType;

typedef struct DisplayTile {
    Rect viewport;
    TileType tileType;
    Color borderColor;
} DisplayTile;

extern Color CreateColor();
extern Color CreateColorRGB(uint8_t r, uint8_t g, uint8_t b);
extern Color CreateColorARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
extern uint32_t ToUint32Color(const Color* const color);

#endif // !_CR_TYPES_H_
