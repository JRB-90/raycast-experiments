#include "scene.h"

Player CreateDefaultPlayer()
{
	Player player;
	player.x = 0.0;
	player.y = 0.0;
	player.theta = 0.0;

	return player;
}

Player CreatePlayer(double x, double y, double theta)
{
	Player player;
	player.x = x;
	player.y = y;
	player.theta = theta;

	return player;
}

void InitDefaultScene(Scene* scene)
{
	Player player = CreateDefaultPlayer();
	scene->player = player;
}
