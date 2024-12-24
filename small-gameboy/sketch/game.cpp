#include "game.h"

GameBase::GameBase(OLED &oled, PS2Button &btn)
{
  this->state = GAME_STATE_INIT;
  this->oled = &oled;
  this->score = 0;
  this->btn = &btn;
}

void GameBase::drawScore()
{
  auto display = this->oled->display;
  display->setCursor(0, 0);
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->print("Score: ");
  display->print(this->score);
}

void GameBase::drawIntroduce(String str)
{
  auto display = this->oled->display;
  display->setTextSize(1);
  display->setTextColor(1);
  display->setCursor(1, 20);
  display->print(str);
}

void SnakeGame::initGame()
{
  snake_length = STARTING_SNAKE_SIZE;
  for (int i = 0; i < snake_length; i++)
  {
    snake[i].x = SNAKE_MAP_UNIT_X_LEN / 2 - i; // 初始化蛇的位置
    snake[i].y = SNAKE_MAP_UNIT_Y_LEN - 1;     // Y坐標固定
  }
  curDir = RIGHT;
  newDir = RIGHT;
}

void SnakeGame::runGame()
{
  switch (this->state)
  {
  case GAME_STATE_INIT:
    randomSeed(analogRead(EMPTY_A_PORT_FOR_RANDOM));
    render();
    if (btn->isClickBtn())
    {
      btn->resetBtn();
      this->state = GAME_STATE_PLAYING;
    }
    delay(FRAME_DELAY);
    break;
  case GAME_STATE_PLAYING:
    // moveTime++;
    // readDirection();
    // if (moveTime >= MOVE_STATE_FRAME_CNT)
    // {
    //   curDir = newDir;
    //   // control
    //   // verify
    //   // response
    //   moveTime = 0;
    // }
    render();
    delay(FRAME_DELAY);
    break;

  case GAME_STATE_END:
    if (btn->isClickBtn())
    {
      btn->resetBtn();
      delay(500);
    }
    break;
  }
}

void SnakeGame::render()
{
  switch (state)
  {
  case GAME_STATE_INIT:
    this->oled->display->clearDisplay();
    this->drawIntroduce("click button to start");
    this->drawScore();
    oled->display->drawRect(SNAKE_MAP_OFFSET_X - 2, SNAKE_MAP_OFFSET_Y - 2, SNAKE_PIECE_SIZE * SNAKE_MAP_UNIT_X_LEN + 2, SNAKE_PIECE_SIZE * SNAKE_MAP_UNIT_Y_LEN + 2, SSD1306_WHITE);
    for (int i = 0; i < snake_length; i++)
    {
      oled->display->drawRect(snake[i].x * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_X, snake[i].y * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_Y, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
    }
    this->oled->display->display();
    break;
  case GAME_STATE_PLAYING:
    this->oled->display->clearDisplay();
    this->drawScore();
    oled->display->drawRect(SNAKE_MAP_OFFSET_X - 2, SNAKE_MAP_OFFSET_Y - 2, SNAKE_PIECE_SIZE * SNAKE_MAP_UNIT_X_LEN + 2, SNAKE_PIECE_SIZE * SNAKE_MAP_UNIT_Y_LEN + 2, SSD1306_WHITE);
    for (int i = 0; i < snake_length; i++)
    {
      oled->display->drawRect(snake[i].x * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_X, snake[i].y * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_Y, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
    }
    this->oled->display->display();
    break;
  case GAME_STATE_END:
    break;
  default:
    break;
  }
}

void AirplaneGame::initGame()
{
  for (int i = 0; i < obstacleCnt; i++)
  {
    this->obstacleList[i] = point{0, SCREEN_HEIGHT + 1};
  }
  this->userPlaceX = SCREEN_WIDTH / 2 - this->playerWidth / 2;
  this->userPlaceY = SCREEN_HEIGHT - this->playerHeight - 1;
}

void AirplaneGame::render()
{
  auto display = this->oled->display;
  switch (this->state)
  {
  case GAME_STATE_INIT:
    display->clearDisplay();
    display->fillRect(this->userPlaceX, this->userPlaceY, this->playerWidth, this->playerHeight, WHITE);
    this->drawScore();
    this->drawIntroduce("click button to start");
    display->display();
    break;
  case GAME_STATE_PLAYING:
    display->clearDisplay();
    for (int i = 0; i < obstacleCnt; i++)
    {
      if (obstacleList[i].y >= 0)
      {
        display->fillRect(obstacleList[i].x, obstacleList[i].y, this->obstacleWidth, this->obstacleHeight, WHITE);
      }
    }
    display->fillRect(this->userPlaceX, this->userPlaceY, this->playerWidth, this->playerHeight, WHITE);
    this->drawScore();
    display->display();
    break;
  case GAME_STATE_END:
    display->clearDisplay();
    this->drawScore();
    this->drawIntroduce("click button to back");
    display->display();
    break;
  default:
    break;
  }
}

void AirplaneGame::runGame()
{
  auto display = this->oled->display;
  switch (this->state)
  {
  case GAME_STATE_INIT:
  {
    randomSeed(analogRead(EMPTY_A_PORT_FOR_RANDOM));
    this->render();
    delay(FRAME_DELAY);
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
    // verify control
    if (userPlaceX < 0)
      userPlaceX = 0;
    if (userPlaceX > SCREEN_WIDTH - this->playerWidth)
      userPlaceX = SCREEN_WIDTH - this->playerWidth;
    if (userPlaceY < 0)
      userPlaceY = 0;
    if (userPlaceY > SCREEN_HEIGHT - this->playerHeight)
      userPlaceY = SCREEN_HEIGHT - this->playerHeight;
    // move enemy
    for (auto i = 0; i < obstacleCnt; i++)
    {
      obstacleList[i].y += MOVE_VELOCITY;
    }
    // verify enemy
    Rectangle r1 = {
        userPlaceX,
        userPlaceY,
        this->playerWidth,
        this->playerHeight,
    };
    for (auto i = 0; i < obstacleCnt; i++)
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
    for (auto i = 0; i < obstacleCnt; i++)
    {
      if (obstacleList[i].y > SCREEN_HEIGHT)
      {
        this->score++;
        obstacleList[i].x = random(SCREEN_WIDTH);
        obstacleList[i].y = random(AIRPLANE_minusHeight, 0);
      }
    }
    this->render();
    // delay
    delay(FRAME_DELAY);
    if (isImpact)
    {
      this->state = GAME_STATE_END;
      this->render();
    }
    break;
  }
  case GAME_STATE_END:
  default:
    oled->printText("error");
    while (true)
    {
    }
    break;
  }
}
