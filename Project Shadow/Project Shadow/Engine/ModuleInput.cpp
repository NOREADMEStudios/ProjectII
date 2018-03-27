#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "../SDL/include/SDL.h"


ModuleInput::ModuleInput() : Module()
{
	name = "input";

	keyboard = new KeyEvent[MAX_KEYS];
	memset(keyboard, { KEY_IDLE }, sizeof(KeyEvent) * MAX_KEYS);
	memset(mouse_buttons, { KEY_IDLE }, sizeof(KeyEvent) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool ModuleInput::Start()
{
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool ModuleInput::PreUpdate()
{
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
			float scale = (int)App->win->GetScale();
			mouse_motion_x = event.motion.xrel / scale;
			mouse_motion_y = event.motion.yrel / scale;
			mouse_x = mouse_x_prev = event.motion.x / scale;
			mouse_y = mouse_y_prev = event.motion.y / scale;
			//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;
		}
	}

	return true;
}

// Called before quitting
bool ModuleInput::CleanUp(pugi::xml_node&)
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool ModuleInput::GetWindowEvent(WindowEvent ev)
{
	return windowEvents[ev];
}

void ModuleInput::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void ModuleInput::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}