#define BAR_SYMBOL '|'
#define BAR_TOP_Y 11 // 右边界中点偏上二点
#define BAR_BOTTOM_Y 15 // 右边界中点偏下二点
#define BAR_STOP 0
#define BAR_DOWN 1
#define BAR_UP -1

struct bar {
  int top_y, bottom_y; // 代表挡板两边端点在Ｘ轴上的位置
  int dir;
};
