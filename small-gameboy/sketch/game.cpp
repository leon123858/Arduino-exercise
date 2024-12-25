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
  generateApple();
}

void SnakeGame::readDirection()
{
  BUTTON_DIRECTORY dir = this->btn->getDir();
  if (dir == MID)
  {
    newDir = curDir;
  }
  else
  {
    newDir = dir;
  }
}

void SnakeGame::generateApple()
{
  unsigned short tryTime = 0;
  bool overlapping;
  do
  {
    overlapping = false;
    if (tryTime > 20)
    {
      for (int i = 0; i < SNAKE_MAP_UNIT_X_LEN; i++)
      {
        for (int j = 0; j < SNAKE_MAP_UNIT_Y_LEN; j++)
        {
          fruit.x = i;
          fruit.y = j;
          overlapping = false;
          for (int i = 0; i < snake_length; i++)
          {
            if (fruit.x == snake[i].x && fruit.y == snake[i].y)
            {
              overlapping = true;
              break;
            }
          }
          if (!overlapping)
          {
            return;
          }
        }
      }
      break;
    }
    else
    {
      tryTime++;
      fruit.x = random(0, SNAKE_MAP_UNIT_X_LEN);
      fruit.y = random(0, SNAKE_MAP_UNIT_Y_LEN);
      for (int i = 0; i < snake_length; i++)
      {
        if (fruit.x == snake[i].x && fruit.y == snake[i].y)
        {
          overlapping = true;
          break;
        }
      }
    }
  } while (overlapping);
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
  {
    // control
    readDirection();
    movingState++;
    if (movingState > SNAKE_MOVING_STATE_CNT)
    {
      // move
      int8_t x = snake[0].x;
      int8_t y = snake[0].y;
      switch (newDir)
      {
      case LEFT:
        x -= 1;
        break;
      case UP:
        y -= 1;
        break;
      case RIGHT:
        x += 1;
        break;
      case DOWN:
        y += 1;
        break;
      }
      if (x == snake[1].x && y == snake[1].y)
      {
        // collision index 1 is user op error, just rollback
        newDir = curDir;
        x = snake[0].x;
        y = snake[0].y;
        switch (curDir)
        {
        case LEFT:
          x -= 1;
          break;
        case UP:
          y -= 1;
          break;
        case RIGHT:
          x += 1;
          break;
        case DOWN:
          y += 1;
          break;
        }
      }
      curDir = newDir;
      // verify
      for (int i = 2; i < snake_length; i++)
      {
        if (x == snake[i].x && y == snake[i].y)
        {
          this->state = GAME_STATE_END;
        }
      }
      if (x < 0 || y < 0 || x >= SNAKE_MAP_UNIT_X_LEN || y >= SNAKE_MAP_UNIT_Y_LEN)
      {
        this->state = GAME_STATE_END;
      }
      // response
      for (int i = snake_length - 1; i > 0; i--)
      {
        snake[i].x = snake[i - 1].x; // 移動蛇身
        snake[i].y = snake[i - 1].y;
      }

      snake[0].x = x; // 更新蛇頭位置
      snake[0].y = y;
      if (fruit.x == snake[0].x && fruit.y == snake[0].y)
      {
        if (snake_length < MAX_SNAKE_LENGTH)
        { // 確保不超過最大長度
          snake_length++;
        }
        generateApple();
        score++;
      }
      // set state
      movingState = 0;
      render();
    }
    delay(FRAME_DELAY);
  }
  break;

  case GAME_STATE_END:
  default:
    oled->printText("error");
    while (true)
    {
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
    oled->display->fillRect(fruit.x * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_X, fruit.y * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_Y, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
    oled->display->drawRect(SNAKE_MAP_OFFSET_X - 2, SNAKE_MAP_OFFSET_Y - 2, SNAKE_PIECE_SIZE * SNAKE_MAP_UNIT_X_LEN + 2, SNAKE_PIECE_SIZE * SNAKE_MAP_UNIT_Y_LEN + 2, SSD1306_WHITE);
    oled->display->fillRect(snake[0].x * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_X, snake[0].y * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_Y, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
    for (int i = 1; i < snake_length; i++)
    {
      oled->display->drawRect(snake[i].x * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_X, snake[i].y * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_Y, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
    }
    this->oled->display->display();
    break;
  case GAME_STATE_END:
    this->oled->display->clearDisplay();
    this->drawScore();
    this->drawIntroduce("collision occur");
    oled->display->fillRect(fruit.x * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_X, fruit.y * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_Y, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
    oled->display->drawRect(SNAKE_MAP_OFFSET_X - 2, SNAKE_MAP_OFFSET_Y - 2, SNAKE_PIECE_SIZE * SNAKE_MAP_UNIT_X_LEN + 2, SNAKE_PIECE_SIZE * SNAKE_MAP_UNIT_Y_LEN + 2, SSD1306_WHITE);
    oled->display->fillRect(snake[0].x * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_X, snake[0].y * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_Y, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
    for (int i = 1; i < snake_length; i++)
    {
      oled->display->drawRect(snake[i].x * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_X, snake[i].y * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_Y, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
    }
    this->oled->display->display();
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
