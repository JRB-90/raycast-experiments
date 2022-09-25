#ifndef _CR_TYPES_H_
#define _CR_TYPES_H_

#include <stdbool.h>
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
    int size;
    int width;
    int height;
    int stride;
    int bitsPP;
    int bytesPP;
    int offset;
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

typedef struct InputState {
    bool quit;
    bool forwards;
    bool backwards;
    bool rotRight;
    bool rotLeft;
    bool right;
    bool left;
    bool toggleDebug;
    bool toggleRenderMode;
} InputState;

extern Color CreateColor();
extern Color CreateColorRGB(uint8_t r, uint8_t g, uint8_t b);
extern Color CreateColorARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
extern uint16_t ToUint16RGB565Color(const Color* const color);
extern uint32_t ToUint32RGBAColor(const Color* const color);
extern InputState DefaultInputState();
extern ScreenBuffer DefaultScreen();

#endif // !_CR_TYPES_H_
