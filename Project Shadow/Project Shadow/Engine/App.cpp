#include <iostream> 

#include "Defs.h"
#include "Log.h"

#include "App.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"


// Constructor
Application::Application(int argc, char* args[]) : argc(argc), args(args)
{
	//PERF_START(ptimer);

	frames = 0;
	want_to_save = want_to_load = false;

	input = new ModuleInput();
	win = new ModuleWindow();
	render = new ModuleRender();
	tex = new ModuleTextures();
	audio = new ModuleAudio();
	/*scene = new j1Scene();
	map = new j1Map();
	entities = new j1Entities();
	collision = new j1Collision();
	pathfinding = new j1PathFinding();
	font = new j1Fonts();
	gui = new j1Gui();
	transition = new j1Transition();*/

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	/*AddModule(map);
	AddModule(scene);
	AddModule(entities);
	AddModule(collision);
	AddModule(pathfinding);
	AddModule(font);
	AddModule(gui);
	AddModule(transition);*/

	// render last to swap buffer
	AddModule(render);

	//PERF_PEEK(ptimer);
}

// Destructor
Application::~Application()
{
	// release modules

	//Where we previously used this to iterate through list items:
	/*p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}*/

	//we must use this structure now with stl:
	//we need to be careful with the use of "iterator" and "reverse_iterator" and use them for their intended purposes (front-to-back and back-to-front)
	for (ModuleList::reverse_iterator item = modules.rbegin(); item != modules.rend(); item++) {
		Release(*item);
	}
	modules.clear();
}

void Application::AddModule(Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool Application::Awake()
{
	//PERF_START(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
		
	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title = app_config.child("title").child_value();
		organization = app_config.child("organization").child_value();
		framerate_cap = app_config.attribute("framerate_cap").as_uint();
	}

	if(ret == true)
	{
		/*p2List_item<j1Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}*/

		for (ModuleList::iterator item = modules.begin(); item != modules.end(); item++) {
			pugi::xml_node node = config.child((*item)->name.c_str());
			ret = (*item)->Awake(node);
		}
	}

	//PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool Application::Start()
{
	//PERF_START(ptimer);

	bool ret = true;
	/*p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}*/

	for (ModuleList::iterator item = modules.begin(); item != modules.end() && ret == true; item++) {
		ret = (*item)->Start();
	}

	//PERF_PEEK(ptimer);
	return ret;
}

// Called each loop iteration
bool Application::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();

	return ret;
}

// ---------------------------------------------
pugi::xml_node Application::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	delta_time = frame_time.Read();
	dt = delta_time / 1000.0f;
	frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if(want_to_save == true)
		SavegameNow();


	if (want_to_reload == true)
	{
		ReloadNow();
	}

	if (want_to_load == true)
		LoadGameNow();

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / ten_sec_timer.ReadSec();
	ten_sec_timer.ReadSec() >= 10 ? ten_sec_timer.Start(), frame_count = 0 : ' '; //This is questionable
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu ",
		avg_fps, last_frame_ms, frames_on_last_update, seconds_since_startup, frame_count);
	win->SetTitle(title);

	delay_time.Start();
	int delay_ms = (1000 / framerate_cap) - last_frame_ms;
	if (delay_ms > 0)
		SDL_Delay(delay_ms);

	uint wait_ms = delay_time.Read();
	LOG("Expected frame delay: %d, Actual frame delay: %d", delay_ms, wait_ms);
}

uint32 Application::GetFramerateCap() const
{
	return framerate_cap;
}

void Application::SetFramerateCap(uint32 cap)
{
	framerate_cap = cap;
}

// Call modules before each loop iteration
bool Application::PreUpdate()
{
	bool ret = true;
	ModuleList::iterator item;
	item = modules.begin();
	Module* pModule = NULL;

	for(item = modules.begin(); item != modules.end() && ret == true; item++)
	{
		pModule = *item;

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool Application::DoUpdate()
{
	bool ret = true;
	ModuleList::iterator item;
	item = modules.begin();
	Module* pModule = NULL;

	for(item = modules.begin(); *item != nullptr && ret == true; item++)
	{
		pModule = *item;

		if(pModule->active == false) {
			continue;
		}

		ret = pModule->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool Application::PostUpdate()
{
	bool ret = true;
	ModuleList::iterator item;
	Module* pModule = NULL;

	for(item = modules.begin(); *item != nullptr && ret == true; item++)
	{
		pModule = *item;

		if(pModule->active == false) {
			continue;
		}

		ret = (*item)->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool Application::CleanUp()
{
	//PERF_START(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;

	config = LoadConfig(config_file);

	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		framerate_cap = app_config.attribute("framerate_cap").as_uint();
	}

	for(ModuleList::reverse_iterator item = modules.rbegin(); item != modules.rend() && ret == true; item++)
	{
		pugi::xml_node node = config.child((*item)->name.c_str());
		ret = (*item)->CleanUp(node);
	}

	config_file.save_file("config.xml");

	//PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int Application::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* Application::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* Application::GetTitle() const
{
	return title.c_str();
}

// ---------------------------------------
const char* Application::GetOrganization() const
{
	return organization.c_str();
}

// Load / Save
void Application::LoadGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
}

// ---------------------------------------
void Application::SaveGame() const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
}

// ---------------------------------------
void Application::GetSaveGames(std::list<std::string>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool Application::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.c_str());

	if(result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.c_str());

		root = data.child("game_state");

		
		ret = true;
		ModuleList::iterator item;

		for(item = modules.begin(); item != modules.end() && ret == true; item++)
		{
			pugi::xml_node node = root.child((*item)->name.c_str());
			ret = (*item)->Load(node);
		}

		data.reset();
		if(ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (*item != nullptr) ? (*item)->name.c_str() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.c_str(), result.description());

	want_to_load = false;
	return ret;
}

bool Application::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.c_str());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");
	
	ModuleList::const_iterator item = modules.begin();

	for(; item != modules.end() && ret == true; item++)
	{
		pugi::xml_node node = root.append_child((*item)->name.c_str());
		ret = (*item)->Save(node);
		item++;
	}

	if(ret == true)
	{
		data.save_file(save_game.c_str());
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", (*item != nullptr) ? (*item)->name.c_str() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}

bool Application::ReloadNow() {

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;

	want_to_reload = false;

	config = LoadConfig(config_file);

	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title= app_config.child("title").child_value();
		organization = app_config.child("organization").child_value();
	}

	if (ret)
		//ret = tex->CleanUp(config.child(tex->name.c_str()));
		//pathfinding->CleanUp(config.child(pathfinding->name.GetString())) &
		//map->CleanUp(config.child(map->name.GetString())) &
		//scene->CleanUp(config.child(scene->name.GetString())) &
		//entities->CleanUp(config.child(entities->name.GetString())) &
		//audio->CleanUp(config.child(audio->name.GetString())) &
		//font->CleanUp(config.child(font->name.GetString())) &
		//gui->CleanUp(config.child(gui->name.GetString()));

	if (ret)
		//ret = tex->Awake(config.child(tex->name.c_str())) &
		//map->Awake(config.child(map->name.GetString())) &
		//audio->Awake(config.child(audio->name.c_str()));
		//scene->Awake(config.child(scene->name.GetString())) &
		//entities->Awake(config.child(entities->name.GetString())) &
		//font->Awake(config.child(font->name.GetString())) &
		//gui->Awake(config.child(gui->name.GetString()));

	if (ret)
		ret = tex->Start() &&
		//map->Start() &&
		audio->Start();
		//scene->Start() &&
		//entities->Start() &&
		//font->Start() &&
		//gui->Start();

	return ret;
}

float Application::GetTimeScale() const
{
	return time_scale;
}

void Application::SetTimeScale(float ts)
{
	time_scale = ts;
}

void Application::Reload() {
	want_to_reload = true;
}