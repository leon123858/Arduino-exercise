#ifndef _MENU_H_
#define _MEMU_H

#include "ps2btn_api.h"
#include "oled_api.h"

enum MenuItem
{
  SNAKE,
  WALL_BALL,
  DINOSAUR,
  MENU_ITEM_COUNT
};

extern const char *menuItems[];
extern const int menuItemsCount;
extern MenuItem currentSelection;

void moveSelection(PS2Button &btn);
void drawMenu(OLED &oled);
#endif // !_MENU_H_
