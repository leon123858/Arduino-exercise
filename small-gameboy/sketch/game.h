#ifndef _GAME_H_

#include "oled_api.h"
#include "ps2btn_api.h"

typedef struct
{
  int x;
  int y;
  int w;
  int h;
} Rectangle;

struct point
{
  int x;
  int y;
};

struct shortPoint
{
  unsigned char x;
  unsigned char y;
};

// 巨集判斷矩形是否相交
#define RECTANGLES_INTERSECT(rect1, rect2) \
  (rect1.x < rect2.x + rect2.w &&          \
   rect1.x + rect1.w > rect2.x &&          \
   rect1.y < rect2.y + rect2.h &&          \
   rect1.y + rect1.h > rect2.y)

// global
#define MOVE_VELOCITY 4
#define FRAME_DELAY 30
#define EMPTY_A_PORT_FOR_RANDOM A3
// app
#define AIRPLANE_obstacleCnt 10
#define AIRPLANE_minusHeight -50

#define SNAKE_PIECE_SIZE 9
#define SNAKE_MAP_OFFSET_X 10
#define SNAKE_MAP_OFFSET_Y 14
#define SNAKE_MAP_UNIT_X_LEN ((SCREEN_WIDTH - (2 * SNAKE_MAP_OFFSET_X)) / SNAKE_PIECE_SIZE)
#define SNAKE_MAP_UNIT_Y_LEN ((SCREEN_HEIGHT - (2 * SNAKE_MAP_OFFSET_Y)) / SNAKE_PIECE_SIZE)
#define STARTING_SNAKE_SIZE 5
#define MOVE_STATE_FRAME_CNT 3
#define MAX_SNAKE_LENGTH ((SNAKE_MAP_UNIT_X_LEN * SNAKE_MAP_UNIT_Y_LEN)) // base on memory limit, can not too long
#define SNAKE_MOVING_STATE_CNT 10

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
  int movingState = 0;

  void drawScore(void);
  void drawIntroduce(String str);

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

public:
  using GameBase::GameBase;
  void runGame() override;
  void initGame() override;
  void render() override;
};

class WallBallGame : public GameBase
{
};

class AirplaneGame : public GameBase
{
private:
  const unsigned short playerWidth = 8;
  const unsigned short playerHeight = 8;
  const unsigned short obstacleWidth = 2;
  const unsigned short obstacleHeight = 2;
  const unsigned short obstacleCnt = AIRPLANE_obstacleCnt;
  int userPlaceX = 0;
  int userPlaceY = 0;
  struct point obstacleList[AIRPLANE_obstacleCnt]; // obstacleCnt

public:
  using GameBase::GameBase;
  void runGame() override;
  void initGame() override;
  void render() override;
};

#endif // !_GAME_H_
