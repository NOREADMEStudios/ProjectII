#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleAudio.h"
#include "ModuleGUI.h"
#include "UI\Label.h"
#include "../SDL/include/SDL.h"


ModuleInput::ModuleInput() : Module() {
	name = "input";

	keyboard = new KeyEvent[MAX_KEYS];
	mouse_buttons = new KeyEvent[NUM_MOUSE_BUTTONS];
	memset(keyboard, { KEY_IDLE }, sizeof(KeyEvent) * MAX_KEYS);
	memset(mouse_buttons, { KEY_IDLE }, sizeof(KeyEvent) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput() {
	delete[] keyboard;
	delete[] mouse_buttons;
}

// Called before render is available
bool ModuleInput::Awake(pugi::xml_node& config) {
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_GAMEPAD could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		if (!CheckControllers()) {
			LOG("No controllers detected");
		}
	};

	return ret;
}

// Called before the first frame
bool ModuleInput::Start() {
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool ModuleInput::PreUpdate() {
	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	mouse_x = mouse_x_prev;
	mouse_y = mouse_y_prev;

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			keyboard[i].blocked = false;
			if (keyboard[i].keyState == KEY_IDLE)
				keyboard[i].keyState = KEY_DOWN;
			else
				keyboard[i].keyState = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i].keyState == KEY_REPEAT || keyboard[i].keyState == KEY_DOWN)
				keyboard[i].keyState = KEY_UP;
			else
				keyboard[i].keyState = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		mouse_buttons[i].blocked = false;
		if (mouse_buttons[i].keyState == KEY_DOWN)
			mouse_buttons[i].keyState = KEY_REPEAT;

		if (mouse_buttons[i].keyState == KEY_UP)
			mouse_buttons[i].keyState = KEY_IDLE;
	}


	for (uint c = 0; c < MAX_CONTROLLERS; c++) {
		Uint8 controllerState[MAX_BUTTONS];
		for (uint i = 0; i < MAX_BUTTONS; i++) {
			controllerState[i] = SDL_GameControllerGetButton(controllers[c].controller, (SDL_GameControllerButton)i);
		}

		for (uint i = 0; i < MAX_AXIS; i++) {
			controllers[c].axis[i] = (float)SDL_GameControllerGetAxis(controllers[c].controller, (SDL_GameControllerAxis)i) / 32767;
		}


		for (int i = 0; i < MAX_BUTTONS; ++i)
		{
			if (controllerState[i] == B_DOWN)
			{
				if (controllers[c].buttons[i] == B_IDLE)
					controllers[c].buttons[i] = B_DOWN;
				else
					controllers[c].buttons[i] = B_REPEAT;

				ControllerLogInputs(c);
			}
			else
			{
				if (controllers[c].buttons[i] == B_REPEAT || controllers[c].buttons[i] == B_DOWN)
					controllers[c].buttons[i] = B_UP;
				else
					controllers[c].buttons[i] = B_IDLE;
			}
		}
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1].keyState = KEY_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1].keyState = KEY_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;

		case SDL_MOUSEMOTION:
			mouse_motion_x = event.motion.xrel;
			mouse_motion_y = event.motion.yrel;
			mouse_x = mouse_x_prev = event.motion.x;
			mouse_y = mouse_y_prev = event.motion.y;
			//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMOVED:
			//CheckControllers();
			break;
		}
	}

	return true;
}

bool ModuleInput::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->debug = !App->debug;
		App->scenes->EnableDebugLab();
		//App->gui->debugfpslab->Enable(!App->gui->debugfpslab->enabled);
	}

	return true;
}

// Called before quitting
bool ModuleInput::CleanUp(pugi::xml_node&) {
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool ModuleInput::GetWindowEvent(WindowEvent ev) {
	return windowEvents[ev];
}

bool ModuleInput::CheckControllers()
{
	bool ret = false;
	numSticks = SDL_NumJoysticks();
	if (numSticks > 0)
	{
		for (uint i = 0; i < numSticks; i++) {
			if (controllers[i].connected) continue;
			controllers[i].controller = SDL_GameControllerOpen(i);
			if (controllers[i].controller == nullptr)
			{
				LOG("Controller couldn't be initialized SDL_Error: %s\n", SDL_GetError());
			}
			else
			{
				ret = true;
				controllers[i].connected = true;
			}
		}
	}
	return ret;
}

KeyState ModuleInput::GetKey(int id) const {
	if (keyboard[id].blocked)
		return KEY_IDLE;
	return keyboard[id].keyState;
}

KeyState ModuleInput::GetMouseButton(int id) const {
	if (mouse_buttons[id - 1].blocked)
		return KEY_IDLE;
	return mouse_buttons[id - 1].keyState;
}

int ModuleInput::GetNumControllers() const {
	return numSticks;
}

void ModuleInput::GetMousePosition(int& x, int& y) const{
	x = mouse_x;
	y = mouse_y;
}

void ModuleInput::GetMouseMotion(int& x, int& y)const {
	x = mouse_motion_x;
	y = mouse_motion_y;
}

void ModuleInput::BlockMouseEvent(int event_id) {
	mouse_buttons[event_id - 1].blocked = true;
}

void ModuleInput::BlockKeyboardEvent(int event_id) {
	keyboard[event_id].blocked = true;
}

void ModuleInput::BlockMouse() {
	mouse_x = mouse_y = INT_MAX - 2;
}

void ModuleInput::BlockKeyboard() {
	for (uint i = 0; i < MAX_KEYS; i++) {
		keyboard[i].blocked = true;
	}
}



std::list<Input> ModuleInput::GetInputListFromController(int playerNum)const{
	std::list<Input> ret;
	for (int i = 0; i < MAX_BUTTONS; ++i)
	{
		if (controllers[playerNum - 1].buttons[i] == B_DOWN)
		{
			switch (i) {
			case SDL_CONTROLLER_BUTTON_A:
				ret.push_back(Input::BUTTON_A);
				break;
			case SDL_CONTROLLER_BUTTON_B:
				ret.push_back(Input::BUTTON_B);
				break;
			case SDL_CONTROLLER_BUTTON_X:
				ret.push_back(Input::BUTTON_X);
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				ret.push_back(Input::BUTTON_Y);
				break;
			case SDL_CONTROLLER_BUTTON_BACK:
				ret.push_back(Input::BUTTON_SELECT);
				break;
			case SDL_CONTROLLER_BUTTON_GUIDE:
				break;
			case SDL_CONTROLLER_BUTTON_START:
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSTICK:
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
				ret.push_back(Input::L_SHOULDER);
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
				ret.push_back(Input::R_SHOULDER);
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				ret.push_back(Input::UP);
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				ret.push_back(Input::DOWN);
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				ret.push_back(Input::LEFT);
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				ret.push_back(Input::RIGHT);
				break;
			case SDL_CONTROLLER_BUTTON_MAX:
				break;
			}
		}
		else if (controllers[playerNum - 1].buttons[i] == B_REPEAT) {
			switch (i) {
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
				ret.push_back(Input::L_SHOULDER);
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
				ret.push_back(Input::R_SHOULDER);
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				ret.push_back(Input::UP);
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				ret.push_back(Input::DOWN);
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				ret.push_back(Input::LEFT);
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				ret.push_back(Input::RIGHT);
				break;
			}
		}
		for (int i = 0; i < MAX_AXIS; i++)
		{
			if (controllers[playerNum - 1].axis[i] >= 0.5f || controllers[playerNum - 1].axis[i] <= -0.5f)
			{
				switch (i) {
				case 0:
					if (controllers[playerNum - 1].axis[i] > 0.7f)
					{
						ret.push_back(Input::RIGHT);
					}
					else if (controllers[playerNum - 1].axis[i] < -0.7f)
					{
						ret.push_back(Input::LEFT);
					}
					break;
				case 1:
					if (controllers[playerNum - 1].axis[i] > 0.7f)
					{
						ret.push_back(Input::DOWN);
					}
					else if (controllers[playerNum - 1].axis[i] < -0.7f)
					{
						ret.push_back(Input::UP);
					}
					break;
				case 4:	
						ret.push_back(Input::R2);				
					break;
				case 5:				
						ret.push_back(Input::L2);				
					break;
				}
			}
		}
	}
	return ret;
}

bool ModuleInput::GetButtonDown(int controller, int input) const
{
	return controllers[controller - 1].buttons[input] == ButtonState::B_DOWN;
}

Input ModuleInput::GetButtonFromController(int controllerNum, bool joystick ) const {
	Input ret = (Input)SDL_CONTROLLER_BUTTON_INVALID;
	for (int i = 0; i < MAX_BUTTONS; ++i)
	{
		if (controllers[controllerNum - 1].buttons[i] == B_DOWN)
		{
			switch (i) {
			case SDL_CONTROLLER_BUTTON_A:
				ret = Input::BUTTON_A;
				break;
			case SDL_CONTROLLER_BUTTON_B:
				ret = Input::BUTTON_B;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				ret = Input::BUTTON_X;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				ret = Input::BUTTON_Y;
				break;
			case SDL_CONTROLLER_BUTTON_BACK:
				ret = Input::BUTTON_SELECT;
				break;
			case SDL_CONTROLLER_BUTTON_GUIDE:
				break;
			case SDL_CONTROLLER_BUTTON_START:
				ret = Input::BUTTON_START;
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSTICK:
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
				ret = Input::L_SHOULDER;
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
				ret = Input::R_SHOULDER;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				ret = Input::UP;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				ret = Input::DOWN;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				ret = Input::LEFT;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				ret = Input::RIGHT;
				break;
			case SDL_CONTROLLER_BUTTON_MAX:
				break;
			}
		}
		if (joystick) {
			for (int i = 0; i < MAX_AXIS; i++)
			{
				if (controllers[controllerNum - 1].axis[i] >= 0.5f || controllers[controllerNum - 1].axis[i] <= -0.5f)
				{
					switch (i) {
					case 0:
						if (controllers[controllerNum - 1].axis[i] > 0.5f)
						{
							ret = Input::RIGHT;
						}
						else if (controllers[controllerNum - 1].axis[i] < -0.5f)
						{
							ret = Input::LEFT;
						}
						break;
					case 1:
						if (controllers[controllerNum - 1].axis[i] > 0.5f)
						{
							ret = Input::DOWN;
						}
						else if (controllers[controllerNum - 1].axis[i] < -0.5f)
						{
							ret = Input::UP;
						}
						break;
					}
				}
			}
		}
	}
	return ret;
}