#include "scene.h"

Player CreateDefaultPlayer()
{
	Player player;
	player.x = 0.0;
	player.y = 0.0;
	player.theta = 0.0;
	player.color = CreateColorRGB(255, 0, 0);

	return player;
}

Player CreatePlayer(double x, double y, double theta)
{
	Player player;
	player.x = x;
	player.y = y;
	player.theta = theta;
	player.color = CreateColorRGB(255, 0, 0);

	return player;
}

Player CreatePlayerWithColor(double x, double y, double theta, Color color)
{
	Player player;
	player.x = x;
	player.y = y;
	player.theta = theta;
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
		scene->player.y -= 1;
	}

	if (inputState.backwards)
	{
		scene->player.y += 1;
	}

	if (inputState.right)
	{
		scene->player.x += 1;
	}

	if (inputState.left)
	{
		scene->player.x -= 1;
	}
}
