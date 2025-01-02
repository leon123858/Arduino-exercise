#include "menu.h"

const char *menuItems[] = {"Snake", "Ball", "Airplane", "Adjust"};
const int menuItemsCount = MENU_ITEM_COUNT;
MenuItem currentSelection = SNAKE;

void moveSelection(PS2Button &btn)
{
	BUTTON_DIRECTORY dir = btn.getDir();
	switch (dir)
	{
	case LEFT:
		currentSelection = (currentSelection - 1 + menuItemsCount) % menuItemsCount;
		delay(500);
		break;
	case RIGHT:
		currentSelection = (currentSelection + 1) % menuItemsCount;
		delay(500);
		break;
	default:
		break;
	}
}

void drawMenu(OLED &oled)
{
	oled.printMenuItem(menuItems[currentSelection], 2);
}