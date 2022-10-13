#ifndef _CR_SCENE_H_
#define _CR_SCENE_H_

#include "crmath.h"
#include "crlist.h"
#include "crprofile.h"
#include "crtypes.h"

// Data types
typedef struct PlayerSettings {
	double transSpeed;
	double rotSpeed;
	double arrowSize;
	double baseSize;
	double fov;
} PlayerSettings;

typedef struct Player {
	Frame2D frame;
	PlayerSettings settings;
} Player;

typedef struct Wall {
	LineSegment2D line;
	double height;
	Color color;
} Wall;

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
extern Scene* CreateTestScene(
	const PlayerSettings const* settings,
	double size 
);
extern void CleanupScene(Scene* scene); 
extern void UpdatePlayerPosition(
	Scene* const scene,
	double deltaMS,
	const InputState* const inputState,
	CycleProfile* const profile
);

#endif // !_CR_SCENE_H_
