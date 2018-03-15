#ifndef __WINDOW_H__
#define __WINDOW_H__

#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024 
#define SCREEN_SIZE 1
#define MAP_WIDTH 3200
#define MAP_HEIGHT 460
#define CAMERA_LIMIT -2175

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before render is available
	bool Awake(pugi::xml_node&) override;

	// Called before quitting
	bool CleanUp(pugi::xml_node&) override;

	// Changae title
	void SetTitle(const char* new_title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint GetScale() const;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	std::string	title;
	uint		width;
	uint		height;
	uint		scale;
};

#endif // __WINDOW_H__