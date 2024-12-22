#include "game.h"
#include "menu.h"

Game *gameFactory(MenuItem item)
{
  switch (item)
  {
  case SNAKE:
    break;
  case WALL_BALL:
    break;
  case DINOSAUR:
    break;
  default:
    // unexpected error
    break;
  }

  return nullptr;
}