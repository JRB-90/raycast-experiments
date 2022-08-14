#ifndef _SCENE_H_
#define _SCENE_H_

#include "display.h"
#include "input.h"
#include "math.h"

// Data types
struct Player {
	Frame frame;
	Color color;
};

struct Scene {
	struct Player player;
};

// Typedefs
typedef struct Player Player;
typedef struct Scene Scene;

// Function defs
extern Player CreateDefaultPlayer();
extern Player CreatePlayer(double x, double y, double theta);
extern Player CreatePlayerWithColor(double x, double y, double theta, Color color);
extern void InitDefaultScene(Scene* scene);
extern void UpdatePlayerPosition(Scene* scene, InputState inputState);

#endif // !_SCENE_H_
