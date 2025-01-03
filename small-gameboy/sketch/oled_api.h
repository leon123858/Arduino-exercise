#ifndef _OLED_H
#define _OLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1         // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_REG_ADDRESS 0x3C // fixed by vender
#define SCREEN_WIDTH 128      // OLED 寬度像素
#define SCREEN_HEIGHT 64      // OLED 高度像素

class OLED
{
private:
public:
  Adafruit_SSD1306 *display = nullptr;

  OLED();
  ~OLED();

  void init();
  void printText(String str);
  void drawArrow(int x, int y, bool isLeft);

  void printMenuItem(String str, int size);
};

#endif // !_OLED_H