#include <SPI.h>
#include <Wire.h>

#define XY_MIDDLE 450
#define XY_MIDDLE_RANGE 10

#define VERT_PIN A0
#define HORZ_PIN A1
#define SEL_PIN 2
#define INTERRUPT_TRIGGER_TYPE FALLING

#define ABS(x) (x > 0 ? x : -x)
#define NORMALIZE(x, m) (x > m ? x - m :)

/**
 * @brief ps2 button example
 *
 * This function reads the PS2 button direction and prints the corresponding text to the OLED display.
 * It also checks if the button is pressed and prints "interrupt" if it is.
 *
 * @code
 * BUTTON_DIRECTORY ret = btn.getDir();
 * switch (ret) {
 *   case LEFT:
 *     oled.printText("left");
 *     break;
 *   case RIGHT:
 *     oled.printText("right");
 *     break;
 *   case MID:
 *     oled.printText("mid");
 *     break;
 *   case UP:
 *     oled.printText("up");
 *     break;
 *   case DOWN:
 *     oled.printText("down");
 *     break;
 *   default:
 *     // un expected error
 *     break;
 * }
 *
 * if (btn.isClickBtn()) {
 *   oled.printText("interrupt");
 *   delay(1000);
 *   btn.resetBtn();
 * }
 * @endcode
 */

enum BUTTON_DIRECTORY
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  MID,
};

class PS2Button
{
private:
  int controlRawX = 0;
  int controlRawY = 0;

  void updateRawBtnDir(void);

public:
  PS2Button(/* args */);
  ~PS2Button();

  void init(void);

  bool isClickBtn(void);
  void resetBtn(void);
  BUTTON_DIRECTORY getDir(void);
};
