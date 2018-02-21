#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Map;
class j1Entities;
class j1Collision;
class j1PathFinding;
class j1Fonts;
class j1Gui;
class j1Transition;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void Reload();
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

	uint32 GetFramerateCap() const;
	void SetFramerateCap(uint32 cap);

	float GetTimeScale()const;
	void SetTimeScale(float ts);

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;
	bool ReloadNow();

public:

	// Modules
	j1Window*			win = nullptr;
	j1Input*			input = nullptr;
	j1Render*			render = nullptr;
	j1Textures*			tex = nullptr;
	j1Audio*			audio = nullptr;
	j1Scene*			scene = nullptr;
	j1Map*				map = nullptr;
	j1Entities*			entities = nullptr;
	j1Collision*		collision = nullptr;
	j1PathFinding*		pathfinding = nullptr;
	j1Fonts*			font = nullptr;
	j1Gui*				gui = nullptr;
	j1Transition*		transition = nullptr;

	bool debug = false;

private:

	p2List<j1Module*>	modules;
	uint				frames = 0;
	float				dt = .0f;
	float				time_scale = 1.0f;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	bool				want_to_reload = false;
	p2SString			load_game = "save_game.xml";
	mutable p2SString	save_game = "save_game.xml";

	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	uint32				framerate_cap = 0;
	uint32				delta_time = 0;
	j1Timer				delay_time;
	j1Timer				startup_time;
	j1Timer				ten_sec_timer;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
};

extern j1App* App; // No student is asking me about that ... odd :-S

#endif