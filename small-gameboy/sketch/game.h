#ifndef _GAME_H_
#define _GAME_H_

#include "oled_api.h"
#include "ps2btn_api.h"

struct shortPoint
{
	short x : 8;
	short y : 8;
};

// global
#define MOVE_VELOCITY 4
#define FRAME_DELAY 30
#define EMPTY_A_PORT_FOR_RANDOM A3
// app
#define AIRPLANE_obstacleCnt 10
#define AIRPLANE_minusHeight -50
#define AIRPLANE_WIDTH 6
#define AIRPLANE_MIN_WIDTH 2
#define AIRPLANE_MAX_WIDTH 10
#define AIRPLANE_LEN 12
#define AIRPLANE_MAX_LEN 20
#define AIRPLANE_MIN_LEN 4
#define AIRPLANE_STATE_MID 0
#define AIRPLANE_STATE_UP 1
#define AIRPLANE_STATE_DOWN 2
#define AIRPLANE_STATE_LEFT 3
#define AIRPLANE_STATE_RIGHT 4

#define SNAKE_PIECE_SIZE 9
#define SNAKE_MAP_OFFSET_X 10
#define SNAKE_MAP_OFFSET_Y 14
#define SNAKE_MAP_UNIT_X_LEN ((SCREEN_WIDTH - (2 * SNAKE_MAP_OFFSET_X)) / SNAKE_PIECE_SIZE)
#define SNAKE_MAP_UNIT_Y_LEN ((SCREEN_HEIGHT - (2 * SNAKE_MAP_OFFSET_Y)) / SNAKE_PIECE_SIZE)
#define STARTING_SNAKE_SIZE 5
#define MOVE_STATE_FRAME_CNT 3
#define MAX_SNAKE_LENGTH ((SNAKE_MAP_UNIT_X_LEN * SNAKE_MAP_UNIT_Y_LEN) - 5) // base on memory limit, can not too long
#define SNAKE_MOVING_STATE_CNT 500

#define BRICK_HEIGHT_COUNT (3 - 1)
#define BRICK_WIDTH_COUNT (9)
#define BRICK_COUNT (BRICK_HEIGHT_COUNT * BRICK_WIDTH_COUNT)
#define BRICK_GAME_MOVING_STATE_CNT 15

enum GAME_STATE
{
	GAME_STATE_INIT,
	GAME_STATE_PLAYING,
	GAME_STATE_END,
};

class GameBase
{
public:
	OLED *oled;
	PS2Button *btn;
	GAME_STATE state;
	int score;
	unsigned long globalClock = 0;

	void drawScore(void);
	void drawIntroduce(String str);
	bool asyncDelay(int ms);

	GameBase(OLED &oled, PS2Button &btn);

	virtual void runGame() = 0;
	virtual void initGame() = 0;
	virtual void render() = 0;
};

class SnakeGame : public GameBase
{
private:
	shortPoint snake[MAX_SNAKE_LENGTH]; // 蛇的身體之 x,y 座標
	BUTTON_DIRECTORY curDir;
	BUTTON_DIRECTORY newDir;
	shortPoint fruit;
	int8_t snake_length; // 蛇的長度

	void readDirection();
	void generateApple();
	void drawGame();
	void getNewPoint(shortPoint *oldPoint, shortPoint *newPoint);

public:
	using GameBase::GameBase;
	void runGame() override;
	void initGame() override;
	void render() override;
};

class WallBallGame : public GameBase
{
private:
	shortPoint bricksPlace[BRICK_COUNT];
	bool bricksActivity[BRICK_COUNT];
	shortPoint paddle;
	shortPoint ballPlace;
	shortPoint ballVector;

	static const short paddleWidth = 20;
	static const short paddleHeight = 5;
	static const short ballR = 2;
	static const short brickWidth = 12;
	static const short brickHeight = 4;
	static const short brickMidOffset = 1;
	static const short Y_OFFSET = 10;
	static const short X_OFFSET = 4;
	static const short ballSpeed = 2;

	void drawGame();

public:
	using GameBase::GameBase;
	void runGame() override;
	void initGame() override;
	void render() override;
};

class AirplaneGame : public GameBase
{
private:
	const unsigned short obstacleWidth = 2;
	const unsigned short obstacleHeight = 2;
	const unsigned short obstacleCnt = AIRPLANE_obstacleCnt;
	short planeState = AIRPLANE_STATE_MID;
	short userPlaceX = 0;
	short userPlaceY = 0;
	struct shortPoint obstacleList[AIRPLANE_obstacleCnt]; // obstacleCnt

	void drawAirplane(Adafruit_SSD1306 *display);

public:
	using GameBase::GameBase;
	void runGame() override;
	void initGame() override;
	void render() override;
};

class AdjustSession : public GameBase
{
private:
public:
	using GameBase::GameBase;
	void initGame() override;
	void runGame() override;
	void render() override;
};

#endif // !_GAME_H_
