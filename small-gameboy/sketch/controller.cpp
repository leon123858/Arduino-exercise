#include "controller.h"
#include "menu.h"

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
    break;
  case Menu:
    moveSelection(this->btn);
    drawMenu(this->oled);
    if (btn.isClickBtn())
    {
      btn.resetBtn();
      state = Game;
    }
    break;
  case Game:
    // render to game
    oled.printText(menuItems[currentSelection]);
    break;
  case Result:
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
