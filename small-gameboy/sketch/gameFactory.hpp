#ifndef _GAME_FACTORY_
#define _GAME_FACTORY_

#include "game.h"

GameBase *gameFactory(MenuItem item, OLED &oled, PS2Button &btn)
{
	switch (item)
	{
	case SNAKE:
		return new SnakeGame(oled, btn);
	case BALL:
		return new WallBallGame(oled, btn);
	case AIRPLANE:
		return new AirplaneGame(oled, btn);
	case ADJUST:
		return new AdjustSession(oled, btn);
	default:
		return nullptr;
	}
}

#endif // !_GAME_FACTORY_
