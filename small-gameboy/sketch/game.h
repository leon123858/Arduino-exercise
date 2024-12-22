enum GAME_STATE
{
  init,
  play,
  end,
};

class Game
{
public:
  GAME_STATE state;
  int gameResult;

private:
  Game();
  ~Game();

  void Serve();
};

class SnakeGame : public Game
{
};

class WallBallGame : public Game
{
};