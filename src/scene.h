#ifndef _SCENE_H_
#define _SCENE_H_

// Data types
struct Player {
	double x;
	double y;
	double theta;
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
extern void InitDefaultScene(Scene* scene);

#endif // !_SCENE_H_
