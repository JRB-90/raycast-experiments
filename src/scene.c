#include "scene.h"
#include "crayconsts.h"

Player CreateDefaultPlayer()
{
	Player player;
	player.frame = CreateDefaultFrame();
	player.color = CreateColorRGB(255, 0, 0);

	return player;
}

Player CreatePlayer(double x, double y, double theta)
{
	Player player;
	player.frame = CreateFrame(x, y, theta);
	player.color = CreateColorRGB(255, 0, 0);

	return player;
}

Player CreatePlayerWithColor(double x, double y, double theta, Color color)
{
	Player player;
	player.frame = CreateFrame(x, y, theta);
	player.color = color;

	return player;
}

void InitDefaultScene(Scene* scene)
{
	Player player = CreateDefaultPlayer();
	scene->player = player;
}

void UpdatePlayerPosition(Scene* scene, InputState inputState)
{
	if (inputState.forwards)
	{
		Vertex offset = CalculateOffsetPoint(scene->player.frame, TRANS_SPEED);
		scene->player.frame.x += offset.x;
		scene->player.frame.y += offset.y;
	}

	if (inputState.backwards)
	{
		Vertex offset = CalculateOffsetPoint(scene->player.frame, -TRANS_SPEED);
		scene->player.frame.x += offset.x;
		scene->player.frame.y += offset.y;
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
