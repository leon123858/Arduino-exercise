#include "game.h"

GameBase *gameFactory(MenuItem item, OLED &oled, PS2Button &btn)
{
  switch (item)
  {
  case SNAKE:
    break;
  case WALL_BALL:
    break;
  case DINOSAUR:
    break;
  case AIRPLANE:
    GameBase *g = new AirplaneGame(oled, btn);
    return g;
  default:
    // unexpected error
    break;
  }

  return nullptr;
}