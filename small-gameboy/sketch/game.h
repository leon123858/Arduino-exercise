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

// 巨集判斷矩形是否相交
#define RECTANGLES_INTERSECT(rect1, rect2) \
  (rect1.x < rect2.x + rect2.w &&          \
   rect1.x + rect1.w > rect2.x &&          \
   rect1.y < rect2.y + rect2.h &&          \
   rect1.y + rect1.h > rect2.y)

// global
#define MOVE_VELOCITY 4
#define FRAME_DELAY 30
// app
#define AIRPLANE_obstacleCnt 10
#define AIRPLANE_minusHeight -50

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

  void drawScore(void);
  void drawIntroduce(String str);

  GameBase(OLED &oled, PS2Button &btn);

  virtual void runGame() = 0;
  virtual void initGame() = 0;
  virtual void render() = 0;
};

class SnakeGame : public GameBase
{
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
  void AirplaneGame::runGame() override;
  void AirplaneGame::initGame() override;
  void AirplaneGame::render() override;
};

#endif // !_GAME_H_
