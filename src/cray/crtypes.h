#ifndef _CR_TYPES_H_
#define _CR_TYPES_H_

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

#endif // !_CR_TYPES_H_
