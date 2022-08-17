#include "scene.h"
#include "crayconsts.h"

Player CreateDefaultPlayer()
{
	Player player =
	{
		.frame = CreateFrame2D(0.0, 0.0, 0.0),
		.color = CreateColorRGB(255, 0, 0)
	};

	return player;
}

Scene CreateDefaultScene()
{
	Scene scene =
	{
		.player = CreateDefaultPlayer(),
		.walls = CreateDoubleLinkedList()
	};

	return scene;
}

void UpdatePlayerPosition(Scene* const scene, InputState inputState)
{
	if (inputState.forwards)
	{
		Vector2D worldForward = { .x = 0.0, .y = -1.0 };
		Vector2D lookDir =
			FindLookVector(
				worldForward,
				scene->player.frame.theta
			);

		lookDir = Vec2DNormalise(lookDir);

		scene->player.frame.position =
			AddVec2DToPoint2D(
				scene->player.frame.position,
				Vec2DMul(lookDir, TRANS_SPEED)
			);
	}

	if (inputState.backwards)
	{
		Vector2D worldForward = { .x = 0.0, .y = -1.0 };
		Vector2D lookDir =
			FindLookVector(
				worldForward,
				scene->player.frame.theta
			);

		lookDir = Vec2DNormalise(lookDir);

		scene->player.frame.position =
			AddVec2DToPoint2D(
				scene->player.frame.position,
				Vec2DMul(lookDir, TRANS_SPEED)
			);
	}

	if (inputState.right)
	{
		scene->player.frame.theta += ROT_SPEED;
	}

	if (inputState.left)
	{
		scene->player.frame.theta -= ROT_SPEED;
	}
}
