#include "./ps2btn_api.h"

volatile bool PS2SelPress;
static int naturalMiddle_x = XY_MIDDLE_DEFAULT;
static int naturalMiddle_y = XY_MIDDLE_DEFAULT;

PS2Button::PS2Button(/* args */)
{
	this->controlRawX = naturalMiddle_x;
	this->controlRawY = naturalMiddle_y;
}

PS2Button::~PS2Button()
{
}

void PS2Button::updateRawBtnDir(void)
{
	int vert = analogRead(VERT_PIN);
	int horz = analogRead(HORZ_PIN);
	this->controlRawX = vert;
	this->controlRawY = horz;
}

bool PS2Button::isClickBtn(void)
{
	return PS2SelPress;
}

void PS2Button::resetBtn(void)
{
	PS2SelPress = false;
}

static void button_interrupt_handler()
{
	PS2SelPress = true;
}

void PS2Button::init(void)
{
	// interrupt 所需設置
	pinMode(SEL_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(SEL_PIN),
					button_interrupt_handler,
					INTERRUPT_TRIGGER_TYPE);
	this->resetBtn();
}

void PS2Button::adjust(void)
{
	delay(1000);
	updateRawBtnDir();
	naturalMiddle_x = this->controlRawX;
	naturalMiddle_y = this->controlRawY;
}

BUTTON_DIRECTORY PS2Button::getDir(void)
{
	this->updateRawBtnDir();

	int newX = this->controlRawX - naturalMiddle_x;
	int newY = this->controlRawY - naturalMiddle_y;

	if (ABS(newX) < XY_MIDDLE_RANGE && ABS(newY) < XY_MIDDLE_RANGE)
	{
		return MID;
	}

	if (ABS(newX) > ABS(newY))
	{
		if (newX > 0)
		{
			return RIGHT;
		}
		else
		{
			return LEFT;
		}
	}
	else
	{
		if (newY > 0)
		{
			return DOWN;
		}
		else
		{
			return UP;
		}
	}
}