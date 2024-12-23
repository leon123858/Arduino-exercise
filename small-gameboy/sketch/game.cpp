#include "game.h"

GameBase::GameBase(OLED &oled, PS2Button &btn)
{
  this->state = GAME_STATE_INIT;
  this->oled = &oled;
  this->score = 0;
  this->btn = &btn;
}

void AirplaneGame::runGame()
{
  auto display = this->oled->display;
  switch (this->state)
  {
  case GAME_STATE_INIT:
  {
    this->userPlaceX = SCREEN_WIDTH / 2 - this->playerWidth / 2;
    this->userPlaceY = SCREEN_HEIGHT - this->playerHeight - 1;
    randomSeed(analogRead(0));
    // initView render
    display->clearDisplay();
    // 繪製玩家
    display->fillRect(this->userPlaceX, this->userPlaceY, this->playerWidth, this->playerHeight, WHITE);
    // 繪製分數
    display->setCursor(0, 0);
    display->setTextSize(1);
    display->setTextColor(WHITE);
    display->print("Score: ");
    display->print(this->score);
    // introduction
    display->setTextSize(1);
    display->setTextColor(1);
    display->setCursor(1, 20);
    display->print("click button to start");
    // render
    display->display();
    delay(10);
    if (this->btn->isClickBtn())
    {
      this->btn->resetBtn();
      this->state = GAME_STATE_PLAYING;
    }
    break;
  }
  case GAME_STATE_PLAYING:
  {
    bool isImpact = false;
    // control
    switch (this->btn->getDir())
    {
    case LEFT:
      userPlaceX -= MOVE_VELOCITY;
      break;
    case RIGHT:
      userPlaceX += MOVE_VELOCITY;
      break;
    case UP:
      userPlaceY -= MOVE_VELOCITY;
      break;
    case DOWN:
      userPlaceY += MOVE_VELOCITY;
      break;
    default:
      break;
    }
    // verify
    if (userPlaceX < 0)
      userPlaceX = 0;
    if (userPlaceX > SCREEN_WIDTH - this->playerWidth)
      userPlaceX = SCREEN_WIDTH - this->playerWidth;
    if (userPlaceY < 0)
      userPlaceY = 0;
    if (userPlaceY > SCREEN_HEIGHT - this->playerHeight)
      userPlaceY = SCREEN_HEIGHT - this->playerHeight;
    // move enemy
    for (auto i = 0; i < 10; i++)
    {
      obstacleList[i].y += MOVE_VELOCITY;
    }
    Rectangle r1 = {
        userPlaceX,
        userPlaceY,
        this->playerWidth,
        this->playerHeight,
    };
    for (auto i = 0; i < 10; i++)
    {
      Rectangle r2 = {
          obstacleList[i].x,
          obstacleList[i].y,
          this->obstacleWidth,
          this->obstacleHeight,
      };
      if (RECTANGLES_INTERSECT(r1, r2))
      {
        isImpact = true;
      }
    }
    for (auto i = 0; i < 10; i++)
    {
      if (obstacleList[i].y > SCREEN_HEIGHT)
      {
        this->score++;
        obstacleList[i].x = random(SCREEN_WIDTH);
        obstacleList[i].y = random(-50, 0);
      }
    }
    // render
    display->clearDisplay();
    for (int i = 0; i < 10; i++)
    {
      if (obstacleList[i].y >= 0)
      {
        display->fillRect(obstacleList[i].x, obstacleList[i].y, this->obstacleWidth, this->obstacleHeight, WHITE);
      }
    }
    display->fillRect(this->userPlaceX, this->userPlaceY, this->playerWidth, this->playerHeight, WHITE);
    // 繪製分數
    display->setCursor(0, 0);
    display->setTextSize(1);
    display->setTextColor(WHITE);
    display->print("Score: ");
    display->print(this->score);
    // render
    display->display();
    // delay
    delay(30);
    if (isImpact)
    {
      display->clearDisplay();
      display->setCursor(0, 0);
      display->setTextSize(1);
      display->setTextColor(WHITE);
      display->print("Score: ");
      display->print(this->score);
      // intro
      display->setTextSize(1);
      display->setTextColor(1);
      display->setCursor(1, 20);
      display->print("click button to back");
      // render
      display->display();
      this->state = GAME_STATE_END;
    }
    break;
  }
  case GAME_STATE_END:
    // unexpected error
    break;
  default:
    // unexpected error
    break;
  }
}
