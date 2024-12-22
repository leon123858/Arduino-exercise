#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "./oled_api.h"

OLED::OLED()
{
  this->display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
}

OLED::~OLED()
{
  delete this->display;
}

void OLED::init()
{
  // 偵測是否安裝好OLED了
  if (!this->display->begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // 一般1306 OLED的位址都是0x3C
#if (IS_DEBUG)
    Serial.println(F("SSD1306 allocation failed"));
#endif
    for (;;)
      ; // Don't proceed, loop forever
  }
  this->display->clearDisplay(); // 清除畫面
}

void OLED::printText(String str)
{
  this->display->clearDisplay();
  this->display->setTextSize(1);   // 設定文字大小
  this->display->setTextColor(1);  // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  this->display->setCursor(5, 25); // 設定起始座標
  this->display->print(str);       // 要顯示的字串
  this->display->display();        // 要有這行才會把文字顯示出來
}

void OLED::printMenuItem(String str, int size)
{
  // 計算框框的大小
  int boxWidth = str.length() * 12 + 8; // 每個字母約佔 6 像素寬，額外加上邊框               // 每行的高度
  int boxX = 0;                         // 框框的 X 坐標
  int boxY = 20;                        // 框框的 Y 坐標

  this->display->clearDisplay();
  this->display->setTextSize(1);
  this->display->setTextColor(1);
  this->display->setCursor(20, 0);
  this->display->print(" click button ");
  this->drawArrow(0, 0, true);                          // 左箭頭
  this->drawArrow(SCREEN_WIDTH - 8, 0, false);          // 右箭頭
  this->display->drawRect(boxX, boxY, boxWidth, 25, 1); // 繪製選中框框
  this->display->setTextSize(size);                     // 設定文字大小
  this->display->setTextColor(1);                       // 1:OLED預設的顏色(這個會依該OLED的顏色來決定)
  this->display->setCursor(5, 25);                      // 設定起始座標
  this->display->print(str);                            // 要顯示的字串
  this->display->display();                             // 要有這行才會把文字顯示出來
}

void OLED::drawArrow(int x, int y, bool isLeft)
{
  if (isLeft)
  {
    // 繪製左箭頭
    this->display->drawLine(x, y + 4, x + 6, y, SSD1306_WHITE);     // 上邊
    this->display->drawLine(x, y + 4, x + 6, y + 8, SSD1306_WHITE); // 下邊
    this->display->drawLine(x + 6, y, x + 6, y + 8, SSD1306_WHITE); // 直線部分
  }
  else
  {
    // 繪製右箭頭
    this->display->drawLine(x + 2, y + 4, x - 4, y, SSD1306_WHITE);     // 上邊
    this->display->drawLine(x + 2, y + 4, x - 4, y + 8, SSD1306_WHITE); // 下邊
    this->display->drawLine(x - 4, y, x - 4, y + 8, SSD1306_WHITE);     // 直線部分
  }
}