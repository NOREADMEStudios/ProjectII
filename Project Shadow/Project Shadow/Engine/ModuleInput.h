#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "Character.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
#define MAX_KEYS 300
#define MAX_BUTTONS 16
#define MAX_AXIS 6
#define MAX_CONTROLLERS 4
//#define LAST_KEYS_PRESSED_BUFFER 50

struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;

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

enum ButtonState
{
	B_IDLE = 0,
	B_DOWN,
	B_REPEAT,
	B_UP
};

struct KeyEvent {
	KeyState keyState = KEY_IDLE;
	bool blocked = false;
};

class Controller {
public:
	ButtonState buttons[MAX_BUTTONS];
	float axis[MAX_AXIS];
	bool connected = false;
	SDL_GameController* controller = nullptr;
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

	//
	bool CheckControllers();
	void ControllerLogInputs(uint controller) {
		//for (uint c = 0; c < MAX_CONTROLLERS; c++) {
		if (controllers[controller].connected) {
			for (uint i = 0; i < MAX_BUTTONS; i++) {
				LOG("Button %d, input: %d", i, controllers[controller].buttons[i]);
			}

			for (uint i = 0; i < MAX_AXIS; i++) {
				LOG("Axis %d, input: %f", i, controllers[controller].axis[i]);
			}
		}
		else {
			LOG("Controller %d disconnected", controller);
		}
		//}
	}

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const;
	KeyState GetMouseButton(int id) const;

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y) const ;
	void GetMouseMotion(int& x, int& y)const;

	int GetNumControllers() const;

	void BlockMouse();
	void BlockKeyboard();

	void BlockMouseEvent(int event_id);
	void BlockKeyboardEvent(int event_id);

	std::list<Input> FirstPlayerConfig();
	std::list<Input> SecondPlayerConfig();

	Input GetButtonFromController(int controllerNum) const;
	bool GetButtonDown(int controller, int input) const;
	std::list<Input> ControllerPlayerConfig(int playerNum) const;

private:

	int			numSticks;
	bool		windowEvents[WE_COUNT];
	KeyEvent*	keyboard;
	KeyEvent*	mouse_buttons;
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x, mouse_x_prev;
	int			mouse_y, mouse_y_prev;

	Controller controllers[MAX_CONTROLLERS];
};

#endif // __INPUT_H__