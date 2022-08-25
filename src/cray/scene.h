#ifndef _SCENE_H_
#define _SCENE_H_

#include "display.h"
#include "input.h"
#include "craymath.h"
#include "dlist.h"

// Data types
typedef struct Player {
	Frame2D frame;
	double fov;
} Player;

typedef struct SceneColors {
	Color clearCol;
	Color wallCol;
	Color floorCol;
	Color ceilingColor;
	Color playerCol;
	Color rayCol;
	Color intersectCol;
} SceneColors;

typedef struct Scene {
	Player player;
	Frame2D camera;
	DLList walls;
	SceneColors colors;
} Scene;

// Function defs
extern Player CreateDefaultPlayer();
extern Scene CreateDefaultScene();
extern void UpdatePlayerPosition(Scene* const scene, InputState inputState);

#endif // !_SCENE_H_
