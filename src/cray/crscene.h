#ifndef _CR_SCENE_H_
#define _CR_SCENE_H_

#include "crmath.h"
#include "crlist.h"
#include "crprofile.h"
#include "crtypes.h"

// Data types
typedef struct PlayerSettings {
	float transSpeed;
	float rotSpeed;
	float arrowSize;
	float baseSize;
	float fov;
} PlayerSettings;

typedef struct Player {
	Frame2D frame;
	PlayerSettings settings;
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
	float wallHeight;
	SceneColors colors;
} Scene;

// Function defs
extern Scene* CreateTestScene(
	const PlayerSettings const* settings,
	float wallHeight,
	float size
);
extern void CleanupScene(Scene* scene);
extern void UpdatePlayerPosition(
	Scene* const scene,
	float deltaMS,
	const InputState* const inputState,
	CycleProfile* const profile
);

#endif // !_CR_SCENE_H_
