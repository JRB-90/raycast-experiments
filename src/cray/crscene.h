#ifndef _CR_SCENE_H_
#define _CR_SCENE_H_

#include "crdisplay.h"
#include "crinput.h"
#include "crmath.h"
#include "dlist.h"
#include "crprofile.h"

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
extern Scene* CreateTestScene(double size);
extern void CleanupScene(Scene* const scene);
extern void UpdatePlayerPosition(
	Scene* const scene, 
	const InputState* const inputState,
	CycleProfile* const profile
);

#endif // !_CR_SCENE_H_
