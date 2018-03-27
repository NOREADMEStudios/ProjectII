#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
#define MAX_KEYS 300
//#define LAST_KEYS_PRESSED_BUFFER 50

enum WindowEvent
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

struct KeyEvent {
	KeyState keyState = KEY_IDLE;
	bool blocked = false;
};

class ModuleInput : public Module
{

public:

	ModuleInput();

	// Destructor
	virtual ~ModuleInput();

	// Called before render is available
	bool Awake(pugi::xml_node&) override;

	// Called before the first frame
	bool Start() override;

	// Called each loop iteration
	bool PreUpdate() override;

	// Called before quitting
	bool CleanUp(pugi::xml_node&) override;

	// Gather relevant win events
	bool GetWindowEvent(WindowEvent ev);

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		if (keyboard[id].blocked)
			return KEY_IDLE;
		return keyboard[id].keyState;
	}

	KeyState GetMouseButtonDown(int id) const
	{
		if (mouse_buttons[id - 1].blocked)
			return KEY_IDLE;
		return mouse_buttons[id - 1].keyState;
	}

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);


	bool BlockMouseEvent(int event_id) {
		if (mouse_buttons[event_id - 1].blocked)
			return false;
		mouse_buttons[event_id - 1].blocked = true;
		return true;
	}

	bool BlockKeyboardEvent(int event_id) {
		if (keyboard[event_id].blocked)
			return false;
		keyboard[event_id].blocked = true;
		return true;
	}

	void BlockMouse() {
		mouse_x = mouse_y = INT_MAX - 2;
	}

	void BlockKeyboard() {
		for (uint i = 0; i < MAX_KEYS; i++) {
			keyboard[i].blocked = true;
		}
	}

private:
	bool		windowEvents[WE_COUNT];
	KeyEvent*	keyboard;
	KeyEvent	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x, mouse_x_prev;
	int			mouse_y, mouse_y_prev;
};

#endif // __INPUT_H__