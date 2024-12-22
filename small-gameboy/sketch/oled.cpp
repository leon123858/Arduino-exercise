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