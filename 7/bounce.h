// some settings for the game

#define BLANK ' '
#define BALL_SYMBOL 'O'

#define TOP_EDGE 5
#define BOTTOM_EDGE 20
#define LEFT_EDGE 10
#define RIGHT_EDGE 70

#define TICKS_PER_SEC 50

#define X_INIT_POS 10
#define Y_INIT_POS 10
#define X_TIM 5
#define Y_TIM 8

struct pinball {
  int x_pos, y_pos,
      x_ttm, y_ttm,
      x_ttg, y_ttg,
      x_dir, y_dir;
  char symbol;
};
