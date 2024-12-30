#ifndef _GAME_FACTORY_
#define _GAME_FACTORY_

#include "game.h"

GameBase *gameFactory(MenuItem item, OLED &oled, PS2Button &btn)
{
	switch (item)
	{
	case SNAKE:
		return new SnakeGame(oled, btn);
	case WALL_BALL:
		return new WallBallGame(oled, btn);
	case DINOSAUR:
		return nullptr;
	case AIRPLANE:
		return new AirplaneGame(oled, btn);
	default:
		return nullptr;
	}
}

#endif // !_GAME_FACTORY_
