#include "game.h"
#include "phy.hpp"

GameBase::GameBase(OLED &oled, PS2Button &btn)
{
	this->state = GAME_STATE_INIT;
	this->oled = &oled;
	this->score = 0;
	this->btn = &btn;
}

bool GameBase::asyncDelay(int ms)
{
	bool ret = false;
	unsigned long curGlobalTime = millis();
	if (curGlobalTime - globalClock > ms)
	{
		ret = true;
		globalClock = millis();
	}
	return ret;
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
		snake[i].y = SNAKE_MAP_UNIT_Y_LEN - 1;	   // Y坐標固定
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

void SnakeGame::getNewPoint(shortPoint *oldPoint, shortPoint *newPoint)
{
	// move
	int8_t x = oldPoint->x;
	int8_t y = oldPoint->y;
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
	default:
		break;
	}
	newPoint->x = x;
	newPoint->y = y;
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
		if (asyncDelay(SNAKE_MOVING_STATE_CNT))
		{
			// move
			shortPoint np;
			getNewPoint(&snake[0], &np);
			if (np.x == snake[1].x && np.y == snake[1].y)
			{
				// collision index 1 is user op error, just rollback
				newDir = curDir;
				getNewPoint(&snake[0], &np);
			}
			curDir = newDir;
			// verify
			for (int i = 2; i < snake_length; i++)
			{
				if (np.x == snake[i].x && np.y == snake[i].y)
				{
					this->state = GAME_STATE_END;
				}
			}
			if (np.x < 0 || np.y < 0 || np.x >= SNAKE_MAP_UNIT_X_LEN || np.y >= SNAKE_MAP_UNIT_Y_LEN)
			{
				this->state = GAME_STATE_END;
			}
			// opt
			if (fruit.x == np.x && fruit.y == np.y)
			{
				if (snake_length < MAX_SNAKE_LENGTH - 3) // magic number
				{										 // 確保不超過最大長度
					snake_length++;
				}
				generateApple();
				score++;
			}
			// response
			for (int i = snake_length; i > 0; i--)
			{
				snake[i].x = snake[i - 1].x; // 移動蛇身
				snake[i].y = snake[i - 1].y;
			}
			snake[0].x = np.x; // 更新蛇頭位置
			snake[0].y = np.y;
			render();
		}
	}
	break;
	case GAME_STATE_END:
		render();
		break;
	default:
		oled->printText("error");
		while (true)
		{
		}
		break;
	}
}

void SnakeGame::drawGame()
{
	oled->display->fillRect(fruit.x * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_X, fruit.y * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_Y, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
	oled->display->drawRect(SNAKE_MAP_OFFSET_X - 2, SNAKE_MAP_OFFSET_Y - 2, SNAKE_PIECE_SIZE * SNAKE_MAP_UNIT_X_LEN + 2, SNAKE_PIECE_SIZE * SNAKE_MAP_UNIT_Y_LEN + 2, SSD1306_WHITE);
	oled->display->fillRect(snake[0].x * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_X, snake[0].y * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_Y, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
	for (int i = 1; i < snake_length; i++)
	{
		oled->display->drawRect(snake[i].x * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_X, snake[i].y * SNAKE_PIECE_SIZE + SNAKE_MAP_OFFSET_Y, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
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
		drawGame();
		this->oled->display->display();
		break;
	case GAME_STATE_PLAYING:
		this->oled->display->clearDisplay();
		this->drawScore();
		drawGame();
		this->oled->display->display();
		break;
	case GAME_STATE_END:
		this->oled->display->clearDisplay();
		this->drawScore();
		this->drawIntroduce("collision occur");
		drawGame();
		this->oled->display->display();
		break;
	default:
		break;
	}
}

void WallBallGame::initGame()
{
	for (int i = 0; i < BRICK_WIDTH_COUNT; i++)
	{
		for (int j = 0; j < BRICK_HEIGHT_COUNT; j++)
		{
			auto index = j * BRICK_WIDTH_COUNT + i;
			bricksActivity[index] = true;
			bricksPlace[index].x = X_OFFSET + brickMidOffset + i * (brickWidth + brickMidOffset);
			bricksPlace[index].y = Y_OFFSET + j * (brickHeight + brickMidOffset);
		}
	}
	paddle.x = SCREEN_WIDTH / 2 - paddleWidth / 2; // 3 is magic number
	paddle.y = SCREEN_HEIGHT - Y_OFFSET - paddleHeight * 2;
	ballPlace.x = SCREEN_WIDTH / 2;
	ballPlace.y = paddle.y - ballR;
	ballVector.x = ballSpeed;
	ballVector.y = ballSpeed;
}

void WallBallGame::runGame()
{
	if (!asyncDelay(BRICK_GAME_MOVING_STATE_CNT))
	{
		return;
	}
	auto display = this->oled->display;
	switch (this->state)
	{
	case GAME_STATE_INIT:
	{
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
		bool isGameOver = false;
		// control
		switch (this->btn->getDir())
		{
		case LEFT:
			paddle.x -= this->ballSpeed * 2;
			break;
		case RIGHT:
			paddle.x += this->ballSpeed * 2;
			break;
		default:
			break;
		}
		// verify control
		if (paddle.x < X_OFFSET)
			paddle.x = X_OFFSET;
		if (paddle.x > SCREEN_WIDTH - X_OFFSET - paddleWidth)
			paddle.x = SCREEN_WIDTH - X_OFFSET - paddleWidth;
		// verify collision (brick, wall, paddle)
		if (ballPlace.y + ballR > paddle.y &&
			ballPlace.x + ballR > paddle.x &&
			ballPlace.x < paddle.x + paddleWidth)
		{
			ballPlace.y = paddle.y - ballR;
			ballVector.y = -ballVector.y; // 反彈
		}
		if (ballPlace.x < X_OFFSET)
		{
			ballPlace.x = X_OFFSET;
			ballVector.x = -ballVector.x; // 反彈
		}
		if (ballPlace.x > SCREEN_WIDTH - X_OFFSET)
		{
			ballPlace.x = SCREEN_WIDTH - X_OFFSET;
			ballVector.x = -ballVector.x; // 反彈
		}
		if (ballPlace.y < Y_OFFSET)
		{
			ballPlace.y = Y_OFFSET;
			ballVector.y = -ballVector.y; // 反彈
		}
		Circle c(ballPlace.x, ballPlace.y, ballR);
		for (int i = 0; i < BRICK_COUNT; i++)
		{
			if (bricksActivity[i])
			{
				Square rectangle(bricksPlace[i].x, bricksPlace[i].y, brickWidth, brickHeight);
				if (checkSATCollision(rectangle, c))
				{
					bricksActivity[i] = false;
					score++;
					point rectangleMidPoint = rectangle.midPoint();
					Vector mid2mid(rectangleMidPoint.x, rectangleMidPoint.y, c.x, c.y);
					for (size_t j = 0; j < rectangle.vecLen; j++)
					{
						if (mid2mid.isIntersect(&rectangle.vecList[j]))
						{
							switch (j)
							{
							case 0:
							case 2:
								ballVector.y = -ballVector.y;
								break;
							case 1:
							case 3:
								ballVector.x = -ballVector.x;
								break;
							default:
								break;
							}
							break;
						}
					}
				}
			}
		}

		// move ball
		ballPlace.x += ballVector.x;
		ballPlace.y += ballVector.y;
		// verify isGameOver (all point, ball fall)
		if (ballPlace.y > SCREEN_HEIGHT - Y_OFFSET)
		{
			isGameOver = true;
		}
		if (score >= BRICK_COUNT)
		{
			isGameOver = true;
		}
		this->render();
		if (isGameOver)
		{
			this->state = GAME_STATE_END;
		}
		break;
	}
	case GAME_STATE_END:
		this->render();
		break;
	default:
		oled->printText("error");
		while (true)
		{
		}
		break;
	}
}

void WallBallGame::drawGame()
{
	// border
	oled->display->drawRect(X_OFFSET, Y_OFFSET, SCREEN_WIDTH - 2 * X_OFFSET, SCREEN_HEIGHT - 2 * Y_OFFSET, SSD1306_WHITE);
	// paddle
	oled->display->fillRect(paddle.x, paddle.y, paddleWidth, paddleHeight, SSD1306_WHITE);
	// ball
	oled->display->fillCircle(ballPlace.x, ballPlace.y, ballR, SSD1306_WHITE);
	// bracket
	for (int i = 0; i < BRICK_COUNT; i++)
	{
		if (!bricksActivity[i])
		{
			continue;
		}
		oled->display->fillRect(bricksPlace[i].x, bricksPlace[i].y, brickWidth, brickHeight, SSD1306_WHITE);
	}
}

void WallBallGame::render()
{
	switch (state)
	{
	case GAME_STATE_INIT:
		this->oled->display->clearDisplay();
		this->drawIntroduce("click button to start");
		this->drawScore();
		drawGame();
		this->oled->display->display();
		break;
	case GAME_STATE_PLAYING:
		this->oled->display->clearDisplay();
		this->drawScore();
		drawGame();
		this->oled->display->display();
		break;
	case GAME_STATE_END:
		this->oled->display->clearDisplay();
		this->drawScore();
		this->drawIntroduce("game stop!");
		drawGame();
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
		this->obstacleList[i] = shortPoint{0, SCREEN_HEIGHT + 1};
	}
	this->userPlaceX = SCREEN_WIDTH / 2;
	this->userPlaceY = SCREEN_HEIGHT - AIRPLANE_MAX_LEN - 1;
}

void AirplaneGame::drawAirplane(Adafruit_SSD1306 *display)
{

	switch (this->planeState)
	{
	case AIRPLANE_STATE_MID:
		display->fillTriangle(userPlaceX, userPlaceY, userPlaceX - AIRPLANE_WIDTH, userPlaceY + AIRPLANE_LEN, userPlaceX + AIRPLANE_WIDTH, userPlaceY + AIRPLANE_LEN, WHITE);
		break;
	case AIRPLANE_STATE_UP:
		display->fillTriangle(userPlaceX, userPlaceY, userPlaceX - AIRPLANE_MIN_WIDTH, userPlaceY + AIRPLANE_MAX_LEN, userPlaceX + AIRPLANE_MIN_WIDTH, userPlaceY + AIRPLANE_MAX_LEN, WHITE);
		break;
	case AIRPLANE_STATE_DOWN:
		display->fillTriangle(userPlaceX, userPlaceY, userPlaceX - AIRPLANE_MAX_WIDTH, userPlaceY + AIRPLANE_MIN_LEN, userPlaceX + AIRPLANE_MAX_WIDTH, userPlaceY + AIRPLANE_MIN_LEN, WHITE);
		break;
	case AIRPLANE_STATE_LEFT:
		display->fillTriangle(userPlaceX, userPlaceY, userPlaceX - AIRPLANE_MAX_WIDTH, userPlaceY + AIRPLANE_LEN, userPlaceX + AIRPLANE_MIN_WIDTH, userPlaceY + AIRPLANE_LEN, WHITE);
		break;
	case AIRPLANE_STATE_RIGHT:
		display->fillTriangle(userPlaceX, userPlaceY, userPlaceX - AIRPLANE_MIN_WIDTH, userPlaceY + AIRPLANE_LEN, userPlaceX + AIRPLANE_MAX_WIDTH, userPlaceY + AIRPLANE_LEN, WHITE);
		break;
	default:
		break;
	}
}

void AirplaneGame::render()
{
	auto display = this->oled->display;
	switch (this->state)
	{
	case GAME_STATE_INIT:
		display->clearDisplay();
		this->drawScore();
		this->drawAirplane(display);
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
		this->drawAirplane(display);
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
			planeState = AIRPLANE_STATE_LEFT;
			break;
		case RIGHT:
			userPlaceX += MOVE_VELOCITY;
			planeState = AIRPLANE_STATE_RIGHT;
			break;
		case UP:
			userPlaceY -= MOVE_VELOCITY;
			planeState = AIRPLANE_STATE_UP;
			break;
		case DOWN:
			userPlaceY += MOVE_VELOCITY;
			planeState = AIRPLANE_STATE_DOWN;
			break;
		case MID:
			planeState = AIRPLANE_STATE_MID;
		default:
			break;
		}
		// verify control
		if (userPlaceX < 0)
			userPlaceX = 0;
		if (userPlaceX > SCREEN_WIDTH - AIRPLANE_MAX_WIDTH)
			userPlaceX = SCREEN_WIDTH - AIRPLANE_MAX_WIDTH;
		if (userPlaceY < 0)
			userPlaceY = 0;
		if (userPlaceY > SCREEN_HEIGHT - AIRPLANE_MAX_LEN)
			userPlaceY = SCREEN_HEIGHT - AIRPLANE_MAX_LEN;
		// move enemy
		for (auto i = 0; i < obstacleCnt; i++)
		{
			obstacleList[i].y += MOVE_VELOCITY;
		}
		// verify enemy
		short x2 = 0;
		short y2 = 0;
		short x3 = 0;
		short y3 = 0;
		switch (this->planeState)
		{
		case AIRPLANE_STATE_MID:
			x2 = userPlaceX - AIRPLANE_WIDTH;
			y2 = userPlaceY + AIRPLANE_LEN;
			x3 = userPlaceX + AIRPLANE_WIDTH;
			y3 = userPlaceY + AIRPLANE_LEN;
			break;
		case AIRPLANE_STATE_UP:
			x2 = userPlaceX - AIRPLANE_MIN_WIDTH;
			y2 = userPlaceY + AIRPLANE_MAX_LEN;
			x3 = userPlaceX + AIRPLANE_MIN_WIDTH;
			y3 = userPlaceY + AIRPLANE_MAX_LEN;
			break;
		case AIRPLANE_STATE_DOWN:
			x2 = userPlaceX - AIRPLANE_MAX_WIDTH;
			y2 = userPlaceY + AIRPLANE_MIN_LEN;
			x3 = userPlaceX + AIRPLANE_MAX_WIDTH;
			y3 = userPlaceY + AIRPLANE_MIN_LEN;
			break;
		case AIRPLANE_STATE_LEFT:
			x2 = userPlaceX - AIRPLANE_MAX_WIDTH;
			y2 = userPlaceY + AIRPLANE_LEN;
			x3 = userPlaceX + AIRPLANE_MIN_WIDTH;
			y3 = userPlaceY + AIRPLANE_LEN;
			break;
		case AIRPLANE_STATE_RIGHT:
			x2 = userPlaceX - AIRPLANE_MIN_WIDTH;
			y2 = userPlaceY + AIRPLANE_LEN;
			x3 = userPlaceX + AIRPLANE_MAX_WIDTH;
			y3 = userPlaceY + AIRPLANE_LEN;
			break;
		default:
			break;
		}
		Triangle t1(userPlaceX, userPlaceY, x2, y2, x3, y3);
		for (auto i = 0; i < obstacleCnt; i++)
		{
			Square s2(obstacleList[i].x, obstacleList[i].y, this->obstacleWidth, this->obstacleHeight);
			bool ret = checkSATCollision(s2, t1);
			if (ret)
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
		}
		break;
	}
	case GAME_STATE_END:
		this->render();
		break;
	default:
		oled->printText("error");
		while (true)
		{
		}
		break;
	}
}

void AdjustSession::initGame() {}

void AdjustSession::runGame()
{
	switch (state)
	{
	case GAME_STATE_INIT:
		this->render();
		if (this->btn->isClickBtn())
		{
			this->btn->resetBtn();
			this->state = GAME_STATE_PLAYING;
		}
		break;
	case GAME_STATE_PLAYING:
		this->render();
		this->btn->adjust();
		this->state = GAME_STATE_END;
		break;
	case GAME_STATE_END:
		this->render();
		break;
	}
}

void AdjustSession::render()
{
	auto display = oled->display;
	switch (state)
	{
	case GAME_STATE_INIT:
		display->clearDisplay();
		this->drawIntroduce("click button to start adjust");
		display->display();
		break;
	case GAME_STATE_PLAYING:
		display->clearDisplay();
		this->drawIntroduce("do not move button");
		display->display();
		break;
	case GAME_STATE_END:
		display->clearDisplay();
		this->drawIntroduce("click button to back");
		display->display();
		break;
	default:
		break;
	}
};