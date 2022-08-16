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

void UpdatePlayerPosition(Scene* scene, InputState inputState)
{
	if (inputState.forwards)
	{
		Point2D offset = 
			CalculateOffsetPoint2D(
				scene->player.frame, 
				TRANS_SPEED
			);

		scene->player.frame.position.x += offset.x;
		scene->player.frame.position.y += offset.y;
	}

	if (inputState.backwards)
	{
		Point2D offset = 
			CalculateOffsetPoint2D(
				scene->player.frame, 
				-TRANS_SPEED
			);

		scene->player.frame.position.x += offset.x;
		scene->player.frame.position.y += offset.y;
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
