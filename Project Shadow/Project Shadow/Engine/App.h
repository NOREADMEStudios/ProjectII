#ifndef __APP_H__
#define __APP_H__

#include <list>
#include "Defs.h"
#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "../PugiXml/src/pugixml.hpp"


typedef std::list<Module*> ModuleList;

// Modules
class ModuleWindow;
class ModuleInput;
class ModuleRender;
class ModuleTextures;
class ModuleAudio;
class ModuleSceneManager;
class j1Map;
class j1Entities;
class j1Collision;
class j1PathFinding;
class j1Fonts;
class j1Gui;
class j1Transition;

class Application
{
public:

	// Constructor
	Application(int argc, char* args[]);

	// Destructor
	virtual ~Application();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void Reload();
	void GetSaveGames(std::list<std::string>& list_to_fill) const;

	uint32 GetFramerateCap() const;
	void SetFramerateCap(uint32 cap);

	float GetTimeScale()const;
	void SetTimeScale(float ts);

private:
	// Sets the provided node with the default structure of the config file
	void CreateDefaultConfigFile(xmlNode&) const;

	// Load config file
	xmlNode LoadConfig(xmlDocument&) const;

	// Checks wether the file structure of the project matches the one specified in the config file or not
	// Returns true if Assets/ folder is present, false otherwise
	// Will create missing folders if Assets/ is present
	bool CheckFileStructure(const xmlNode&) const;

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
	ModuleWindow*			win = nullptr;
	ModuleInput*			input = nullptr;
	ModuleRender*			render = nullptr;
	ModuleTextures*			tex = nullptr;
	ModuleAudio*			audio = nullptr;
	ModuleSceneManager*		scenes = nullptr;
	j1Map*				map = nullptr;
	j1Entities*			entities = nullptr;
	j1Collision*		collision = nullptr;
	j1PathFinding*		pathfinding = nullptr;
	j1Fonts*			font = nullptr;
	j1Gui*				gui = nullptr;
	j1Transition*		transition = nullptr;


	bool debug = false;

private:

	ModuleList			modules;
	uint				frames = 0;
	float				dt = .0f;
	float				time_scale = 1.0f;
	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;
	bool				want_to_reload = false;
	std::string			load_game = "save_game.xml";
	mutable std::string	save_game = "save_game.xml";

	PerfTimer			ptimer;
	uint64				frame_count = 0;
	uint32				framerate_cap = 0;
	uint32				delta_time = 0;
	Timer				delay_time;
	Timer				startup_time;
	Timer				ten_sec_timer;
	Timer				frame_time;
	Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
};

extern Application* App;

#endif