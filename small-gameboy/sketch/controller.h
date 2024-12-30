#if !defined(_CONTROLLER_)
#define _CONTROLLER_

#include "oled_api.h"
#include "ps2btn_api.h"
#include "menu.h"

enum ControllerState
{
	Welcome,
	Menu,
	Game,
	Result
};

class Controller
{
private:
	OLED oled;
	PS2Button btn;
	ControllerState state;

public:
	Controller(/* args */);
	~Controller();

	void Serve(void);
};

#endif // _CONTROLLER_