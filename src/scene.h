#ifndef _SCENE_H_
#define _SCENE_H_

#include "display.h"
#include "input.h"
#include "craymath.h"
#include "dlist/dlist.h"

// Data types
typedef struct Player {
	Frame2D frame;
	Color color;
} Player;

typedef struct Scene {
	Player player;
	DLList walls;
} Scene;

// Function defs
extern Player CreateDefaultPlayer();
extern Scene CreateDefaultScene();
extern void UpdatePlayerPosition(Scene* const scene, InputState inputState);

#endif // !_SCENE_H_
