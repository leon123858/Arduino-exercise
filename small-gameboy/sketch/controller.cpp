#include "controller.h"
#include "menu.h"
#include "gameFactory.hpp"

GameBase *curGame = nullptr;

Controller::Controller(/* args */)
{
  this->state = Welcome;
  this->oled.init();
  this->btn.init();
  this->oled.printText("Welcome To Scouting");
}

Controller::~Controller()
{
}

// should be execute each loop
void Controller::Serve(void)
{
  ControllerState curState = this->state;
  switch (curState)
  {
  case Welcome:
    delay(1500);
    this->state = Menu;
    btn.resetBtn();
    break;
  case Menu:
    moveSelection(this->btn);
    drawMenu(this->oled);
    if (btn.isClickBtn())
    {
      btn.resetBtn();
      curGame = gameFactory(currentSelection, this->oled, this->btn);
      if (curGame == nullptr)
      {
        oled.printText("null game");
        while (true)
        {
        }
      }
      curGame->initGame();
      this->state = Game;
    }
    break;
  case Game:
    // render to game
    curGame->runGame();
    if (curGame->state == GAME_STATE_END)
    {
      this->state = Result;
      btn.resetBtn();
    }
    break;
  case Result:
    curGame->runGame();
    if (btn.isClickBtn())
    {
      btn.resetBtn();
      delete curGame;
      this->state = Menu;
    }
    break;
  default:
    // unexpected error
    this->oled.printText("error");
    while (true)
    {
    };
    break;
  }
}
