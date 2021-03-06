#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "Character.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
#define MAX_KEYS 300
#define MAX_BUTTONS 15
#define MAX_AXIS 6
#define MAX_CONTROLLERS 4
#define EDITABLE_BUTTONS 8
//#define LAST_KEYS_PRESSED_BUFFER 50

struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;


enum Input
{
	NONEINPUT,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	L_SHOULDER,
	R_SHOULDER,
	L2,
	R2,
	BUTTON_START,
	BUTTON_SELECT,
};

struct InputButton
{
	Input input;
	std::string name;
	int tag;
	CharInput char_input;
	InputButton( Input i, std::string n, int _tag, CharInput ci )
	{
		input = i;
		name = n;
		tag = _tag;
		char_input = ci;
	}
	InputButton(InputButton* i)
	{
		input = i->input;
		name = i->name;
		tag = i->tag;
		char_input =i->char_input;
	}
};

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

	bool Update(float dt) override;

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
	void BlockAllControllers();

	void BlockMouseEvent(int event_id);
	void BlockKeyboardEvent(int event_id);
	void BlockControllerEvent(int controller, int event_id);

	void BlockAllInput();

	//std::list<CharInput> FirstPlayerConfig();
	//std::list<CharInput> SecondPlayerConfig();

	Input GetButtonFromController(int controllerNum, bool joystick = true ) const;
	bool GetButtonDown(int controller, int input) const;
	std::list<Input> GetInputListFromController(int controllerNum) const;
	void SetBasicConfig();
	void SetDefaultConfig();
	
	InputButton* GetInputButton(Input i);
	void SwapInputs(InputButton* a, InputButton* b);
	std::vector<InputButton*> saved_list;
	std::vector<InputButton*> default_list;
	void ResetConfig();

	bool Load(pugi::xml_node& data)override;
	bool Save(pugi::xml_node&) const override;

private:

	int			numSticks;
	bool		windowEvents[WE_COUNT];
	KeyEvent*	keyboard;
	KeyEvent*	mouse_buttons;
	int			mouse_motion_x;
	int			mouse_motion_y;
	int			mouse_x, mouse_x_prev;
	int			mouse_y, mouse_y_prev;

	void EmptyConfig();
	void CleanUpConfig();
	void LoadFromXML();

	Controller controllers[MAX_CONTROLLERS];
	int savedInputTags[EDITABLE_BUTTONS];
};

#endif // __INPUT_H__