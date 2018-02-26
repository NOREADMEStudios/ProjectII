#include <iostream> 

//#include "p2Defs.h"
//#include "p2Log.h"

#include "App.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"


// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
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
App::~App()
{
	// release modules

	//Where we previously used this to iterate through list items:
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	//we must use this structure now with stl:
	//we need to be careful with the use of "iterator" and "reverse_iterator" and use them for their intended purposes (front-to-back and back-to-front)
	for (std::list<Module*>::reverse_iterator item = modules.rbegin(); item != modules.rend(); item++) {
		RELEASE(*item);
	}
	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
	PERF_START(ptimer);

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
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());
		framerate_cap = app_config.attribute("framerate_cap").as_uint();
	}

	if(ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool App::Start()
{
	PERF_START(ptimer);

	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	PERF_PEEK(ptimer);
	return ret;
}

// Called each loop iteration
bool App::Update()
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
pugi::xml_node App::LoadConfig(pugi::xml_document& config_file) const
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
void App::PrepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	delta_time = frame_time.Read();
	dt = delta_time / 1000.0f;
	frame_time.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
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
	App->win->SetTitle(title);

	delay_time.Start();
	int delay_ms = (1000 / framerate_cap) - last_frame_ms;
	if (delay_ms > 0)
		SDL_Delay(delay_ms);

	uint wait_ms = delay_time.Read();
	LOG("Expected frame delay: %d, Actual frame delay: %d", delay_ms, wait_ms);
}

uint32 App::GetFramerateCap() const
{
	return framerate_cap;
}

void App::SetFramerateCap(uint32 cap)
{
	framerate_cap = cap;
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = pModule->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	PERF_START(ptimer);

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

	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp(config.child(item->data->name.GetString()));
		item = item->prev;
	}

	config_file.save_file("config.xml");

	PERF_PEEK(ptimer);
	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
}

// ---------------------------------------
void App::SaveGame() const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
}

// ---------------------------------------
void App::GetSaveGames(p2List<p2SString>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool App::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.GetString());

	if(result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.GetString());

		root = data.child("game_state");

		p2List_item<j1Module*>* item = modules.start;
		ret = true;

		while(item != NULL && ret == true)
		{
			ret = item->data->Load(root.child(item->data->name.GetString()));
			item = item->next;
		}

		data.reset();
		if(ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (item != NULL) ? item->data->name.GetString() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.GetString(), result.description());

	want_to_load = false;
	return ret;
}

bool App::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.GetString());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	p2List_item<j1Module*>* item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Save(root.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if(ret == true)
	{
		data.save_file(save_game.GetString());
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", (item != NULL) ? item->data->name.GetString() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}

bool App::ReloadNow() {

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
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());
	}

	if (ret)
		ret = tex->CleanUp(config.child(tex->name.GetString())) &
		//pathfinding->CleanUp(config.child(pathfinding->name.GetString())) &
		map->CleanUp(config.child(map->name.GetString())) &
		scene->CleanUp(config.child(scene->name.GetString())) &
		entities->CleanUp(config.child(entities->name.GetString())) &
		//audio->CleanUp(config.child(audio->name.GetString())) &
		font->CleanUp(config.child(font->name.GetString())) &
		gui->CleanUp(config.child(gui->name.GetString()));

	if (ret)
		ret = tex->Awake(config.child(tex->name.GetString())) &
		map->Awake(config.child(map->name.GetString())) &
		//audio->Awake(config.child(audio->name.GetString())) &
		scene->Awake(config.child(scene->name.GetString())) &
		entities->Awake(config.child(entities->name.GetString())) &
		font->Awake(config.child(font->name.GetString())) &
		gui->Awake(config.child(gui->name.GetString()));

	if (ret)
		ret = tex->Start() &&
		map->Start() &&
		//audio->Start() &&
		scene->Start() &&
		entities->Start() &&
		font->Start() &&
		gui->Start();

	return ret;
}

float App::GetTimeScale() const
{
	return time_scale;
}

void App::SetTimeScale(float ts)
{
	time_scale = ts;
}

void App::Reload() {
	want_to_reload = true;
}