#include "crscene.h"
#include <assert.h>
#include <stdlib.h>
#include "crtime.h"

Wall* CreateWall(
	LineSegment2D line, 
	double height, 
	Color color)
{
	Wall* wall = malloc(sizeof(Wall));
	assert(wall != NULL);
	wall->line = line;
	wall->height = height;
	wall->color = color;

	return wall;
}

Scene* CreateTestScene(
	const PlayerSettings const* settings,
	double size)
{
	Scene* scene = malloc(sizeof(Scene));
	assert(scene != NULL);

	scene->player =
	(Player){
		.frame = CreateFrame2D(0.0, 0.0, 0.0),
		.settings =
		(PlayerSettings){
			.arrowSize = settings->arrowSize,
			.baseSize = settings->baseSize,
			.transSpeed = settings->transSpeed,
			.rotSpeed = settings->rotSpeed,
			.fov = settings->fov
		}
	};

	scene->walls = CreateDoubleLinkedList();

	scene->colors =
	(SceneColors){
		.clearCol = CreateColorRGB(0, 0, 0),
		.wallCol = CreateColorRGB(255, 255, 255),
		.floorCol = CreateColorRGB(52, 52, 52),
		.ceilingColor = CreateColorRGB(52, 52, 128),
		.playerCol = CreateColorRGB(0, 0, 255),
		.rayCol = CreateColorRGB(255, 0, 0),
		.intersectCol = CreateColorRGB(0, 255, 0)
	};

	scene->camera =
	(Frame2D){
		.position = {.x = 0.0, .y = 0.0 },
		.theta = 0.0
	};

	double hsize = size / 2.0;

	Point2D p1 = { .x = -size, .y = -hsize };
	Point2D p2 = { .x = -hsize, .y = hsize };
	Point2D p3 = { .x = size, .y = size };
	Point2D p4 = { .x = size, .y = -hsize };
	Point2D p5 = { .x = hsize, .y = -hsize };
	Point2D p6 = { .x = hsize, .y = -size };
	Point2D p7 = { .x = -hsize, .y = -size };
	Point2D p8 = { .x = -hsize, .y = -hsize };

	LineSegment2D l1 = { .p1 = p1, .p2 = p2 };
	LineSegment2D l2 = { .p1 = p2, .p2 = p3 };
	LineSegment2D l3 = { .p1 = p3, .p2 = p4 };
	LineSegment2D l4 = { .p1 = p4, .p2 = p5 };
	LineSegment2D l5 = { .p1 = p5, .p2 = p6 };
	LineSegment2D l6 = { .p1 = p6, .p2 = p7 };
	LineSegment2D l7 = { .p1 = p7, .p2 = p8 };
	LineSegment2D l8 = { .p1 = p8, .p2 = p1 };

	Wall* w1 = CreateWall(l1, 4000.0, CreateColorRGB(255, 0, 0));
	Wall* w2 = CreateWall(l2, 4000.0, CreateColorRGB(0, 255, 0));
	Wall* w3 = CreateWall(l3, 4000.0, CreateColorRGB(0, 0, 255));
	Wall* w4 = CreateWall(l4, 4000.0, CreateColorRGB(255, 255, 255));
	Wall* w5 = CreateWall(l5, 4000.0, CreateColorRGB(255, 0, 0));
	Wall* w6 = CreateWall(l6, 4000.0, CreateColorRGB(0, 255, 0));
	Wall* w7 = CreateWall(l7, 4000.0, CreateColorRGB(0, 0, 255));
	Wall* w8 = CreateWall(l8, 4000.0, CreateColorRGB(255, 255, 255));

	assert(w1 != NULL);
	assert(w2 != NULL);
	assert(w3 != NULL);
	assert(w4 != NULL);
	assert(w5 != NULL);
	assert(w6 != NULL);
	assert(w7 != NULL);
	assert(w8 != NULL);

	PushDLLNode(&scene->walls, w1);
	PushDLLNode(&scene->walls, w2);
	PushDLLNode(&scene->walls, w3);
	PushDLLNode(&scene->walls, w4);
	PushDLLNode(&scene->walls, w5);
	PushDLLNode(&scene->walls, w6);
	PushDLLNode(&scene->walls, w7);
	PushDLLNode(&scene->walls, w8);

	return scene;
}

void CleanupScene(Scene* scene)
{
	if (scene == NULL)
	{
		return;
	}

	DLLNode* current = scene->walls.head;

	while (current != NULL)
	{
		Wall* data = (Wall*)current->data;
		free(data);
		current = current->next;
	}

	ClearDoubleLinkedList(&scene->walls);

	free(scene);
	scene = NULL;
}

void UpdatePlayerPosition(
	Scene* const scene,
	double deltaMS,
	const InputState* const inputState,
	CycleProfile* const profile)
{
	uint64_t updateStartTime = GetTicks();

	const Vector2D worldForward = { .x = 0.0, .y = -1.0 };

	if (inputState->forwards)
	{
		
		Vector2D lookDir =
			FindLookVector(
				&worldForward,
				scene->player.frame.theta
			);

		lookDir = Vec2DNormalise(lookDir);

		scene->player.frame.position =
			AddVec2DToPoint2D(
				scene->player.frame.position,
				Vec2DMul(lookDir, scene->player.settings.transSpeed * deltaMS)
			);
	}

	if (inputState->backwards)
	{
		Vector2D worldForward = { .x = 0.0, .y = -1.0 };
		Vector2D lookDir =
			FindLookVector(
				&worldForward,
				scene->player.frame.theta
			);

		lookDir = Vec2DNormalise(lookDir);

		scene->player.frame.position =
			AddVec2DToPoint2D(
				scene->player.frame.position,
				Vec2DMul(lookDir, -scene->player.settings.transSpeed * deltaMS)
			);
	}

	if (inputState->right)
	{
		Vector2D worldForward = { .x = 0.0, .y = -1.0 };
		Vector2D lookDir =
			FindLookVector(
				&worldForward,
				scene->player.frame.theta + 90.0
			);

		lookDir = Vec2DNormalise(lookDir);

		scene->player.frame.position =
			AddVec2DToPoint2D(
				scene->player.frame.position,
				Vec2DMul(lookDir, scene->player.settings.transSpeed * deltaMS)
			);
	}

	if (inputState->left)
	{
		Vector2D worldForward = { .x = 0.0, .y = -1.0 };
		Vector2D lookDir =
			FindLookVector(
				&worldForward,
				scene->player.frame.theta - 90.0
			);

		lookDir = Vec2DNormalise(lookDir);

		scene->player.frame.position =
			AddVec2DToPoint2D(
				scene->player.frame.position,
				Vec2DMul(lookDir, scene->player.settings.transSpeed * deltaMS)
			);
	}

	if (inputState->rotRight)
	{
		scene->player.frame.theta += scene->player.settings.rotSpeed * deltaMS;
	}

	if (inputState->rotLeft)
	{
		scene->player.frame.theta -= scene->player.settings.rotSpeed * deltaMS;
	}

	profile->updatePlayerTimeMS = GetTimeInMS(GetTicks() - updateStartTime);
}
