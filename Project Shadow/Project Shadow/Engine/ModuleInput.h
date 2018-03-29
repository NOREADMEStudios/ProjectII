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
	KeyState GetKey(int id) const;
	KeyState GetMouseButton(int id) const;

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	void BlockMouse();
	void BlockKeyboard();

	void BlockMouseEvent(int event_id);
	void BlockKeyboardEvent(int event_id);


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