#include <iostream> 
#include <cstdlib>
#include <filesystem>

#include "Defs.h"
#include "Log.h"

#include "App.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleMap.h"
#include "ModuleGUI.h"
#include "ModuleFonts.h"
#include "ModuleSceneManager.h"
#include "ModuleEntityManager.h"
#include "ModuleCollision.h"
#include "ModuleTransition.h"

#include "..\Brofiler\Brofiler.h"

// Constructor
Application::Application(int argc, char* args[]) : argc(argc), args(args)
{
	//PERF_START(ptimer);

	frames = 0;
	want_to_save = want_to_load = false;

	input = new ModuleInput();
	win = new ModuleWindow();
	render = new ModuleRender();
	textures = new ModuleTextures();
	audio = new ModuleAudio();
	scenes = new ModuleSceneManager();
	entities = new ModuleEntityManager();
	map = new ModuleMap();	
	collision = new ModuleCollision();
	/*pathfinding = new j1PathFinding();*/
	font = new ModuleFonts();
	gui = new ModuleGUI();
	transition = new ModuleTransition();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(textures);
	AddModule(audio);
	AddModule(map);
	AddModule(scenes);
	AddModule(entities);
	
	AddModule(collision);
	/*AddModule(pathfinding);*/
	AddModule(font);
	AddModule(gui);
	AddModule(transition);

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
		Utils::Release(*item);
	}
	modules.clear();
}

void Application::AddModule(Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool Application::Awake() {
	//PERF_START(ptimer);
	BROFILER_CATEGORY("App_Awake", Profiler::Color::Red);

	xmlDocument	config_file;
	xmlNode		config;
	xmlNode		appConfig;

	bool ret = false;
		
	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true; 
		appConfig = config.child("app");
		title = appConfig.child("Title").attribute("value").as_string();
		organization = appConfig.child("organisation").attribute("value").as_string();
		framerate_cap = appConfig.attribute("framerateCap").as_uint();
		debug = appConfig.child("debug_mode").attribute("value").as_bool();

		//Checking file structure
		CheckFileStructure(appConfig);
	}

	if(ret == true)
	{
		for (ModuleList::iterator item = modules.begin(); item != modules.end() && ret == true; item++) {
			xmlNode node = config.child((*item)->name.c_str());
			ret = (*item)->Awake(node);
		}
	}

	//PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool Application::Start() {
	//PERF_START(ptimer);
	BROFILER_CATEGORY("App_Start", Profiler::Color::Blue);

	bool ret = true;

	for (ModuleList::iterator item = modules.begin(); item != modules.end() && ret == true; item++) {
		ret = (*item)->Start();
	}

	//PERF_PEEK(ptimer);
	return ret;
}

// Called each loop iteration
bool Application::Update() {
	bool ret = true;

	BROFILER_CATEGORY("App_Update", Profiler::Color::Green);
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true || want_to_quit == true)
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

void Application::CreateDefaultConfigFile(xmlNode & configNode) const {
	xmlNode app = configNode.append_child("app");
	app.append_attribute("framerateCap").set_value(60);
	app.append_child("Title").append_attribute("value") = "Shadow Engine";
	app.append_child("organisation").append_attribute("value") = "NoReadme Studio";
	app.append_child("debug_mode").append_attribute("value") = "0";

	// Assets folder structure
	xmlNode assetsStructure = app.append_child("Assets");
	assetsStructure.append_attribute("folder") = ASSETS_ROOT;
	xmlNode anims = assetsStructure.append_child("Animations");
	anims.append_attribute("folder") = ANIMATIONS_DIR;
	anims.append_child("Characters").append_attribute("folder") = CHARACTERS_DIR;
	anims.append_child("Enemies").append_attribute("folder") = ENEMIES_DIR;

	xmlNode audio = assetsStructure.append_child("Audio");
	audio.append_attribute("folder") = AUDIO_DIR;
	audio.append_child("BGM").append_attribute("folder") = AUDIO_BGM_DIR;
	audio.append_child("FX").append_attribute("folder") = AUDIO_FX_DIR;

	assetsStructure.append_child("Maps").append_attribute("folder") = MAPS_DIR;
	assetsStructure.append_child("Scenes").append_attribute("folder") = SCENES_DIR;

	xmlNode textures = assetsStructure.append_child("Textures");
	textures.append_attribute("folder") = TEXTURES_DIR;
	textures.append_child("Characters").append_attribute("folder") = CHARACTERS_DIR;
	textures.append_child("UI").append_attribute("folder") = UI_DIR;
	textures.append_child("Maps").append_attribute("folder") = MAPS_DIR;

	assetsStructure.append_child("Input").append_attribute("folder") = INPUT_DIR;

	configNode.append_child("renderer").append_child("vsync").append_attribute("value") = "false";
	xmlNode window = configNode.append_child("window");
	xmlNode winRes = window.append_child("resolution");
	winRes.append_attribute("width") = DEFAULT_RESOLUTION_X;
	winRes.append_attribute("height") = DEFAULT_RESOLUTION_Y;
	winRes.append_attribute("scale") = 1.0f;
	window.append_child("fullscreen").append_attribute("value") = 1;
	window.append_child("borderless").append_attribute("value") = 0;
	window.append_child("resizable").append_attribute("value") = 0;
	window.append_child("fullscreenWindow").append_attribute("value") = 0;
	configNode.append_child("scenes").append_attribute("folder") = SCENES_DIR;
	configNode.append_child("map").append_attribute("folder") = MAPS_DIR;
	configNode.append_child("entities").append_attribute("folder") = ENTITIES_DIR;
	configNode.append_child("textures").append_attribute("folder") = TEXTURES_DIR;
	audio = configNode.append_child("audio");
	audio.append_attribute("folder") = AUDIO_DIR;
	audio.append_attribute("volumeFX") = 1.0f;
	audio.append_attribute("volumeBGM") = 1.0f;
	configNode.append_child("input").append_attribute("folder") = INPUT_DIR;
	xmlNode tags = configNode.append_child("collision").append_child("colliderTags");
	xmlNode tag = tags.append_child("tag");
	tag.append_attribute("value") = "default";
	tag.append_attribute("interactions") = "default;default";
}

// ---------------------------------------------
xmlNode Application::LoadConfig(xmlDocument& config_file) const {
	xmlNode ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if (result == NULL) {
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		LOG("Creating default config.xml file...");
		ret = config_file.append_child("config");
		CreateDefaultConfigFile(ret);
		config_file.save_file("config.xml");
	}
	else
		ret = config_file.child("config");

	return ret;
}

void Application::CheckFileStructure(const xmlNode & config) const
{
	namespace filesystem = std::experimental::filesystem;
	xmlNode assets = config.child("Assets");

	std::string path = assets.attribute("folder").as_string();
	if (!filesystem::exists(path)) {
		LOG("Missing folder %s, creating default one", path.c_str());
		filesystem::create_directory(path);
	}

	// Iterate through nodes to check and create if needed all the assets directories
	for (xmlNode iter = assets.first_child(); iter;) {
		path = path + iter.attribute("folder").as_string();
		if (!filesystem::exists(path)) {
			LOG("Missing folder %s, creating default one", path.c_str());
			filesystem::create_directory(path);
		}

		if (iter.first_child() != nullptr) {
			iter = iter.first_child();
		}
		else if (iter.next_sibling() != nullptr) {
			int pos = path.find(iter.attribute("folder").as_string(), 0);
			path = path.substr(0, pos);
			iter = iter.next_sibling();
		}
		else {
			int pos = path.find(iter.attribute("folder").as_string(), 0);
			path = path.substr(0, pos);
			pos = path.find(iter.parent().attribute("folder").as_string());
			path = path.substr(0, pos);
			iter = iter.parent().next_sibling();
		}
	}
}

// ---------------------------------------------
void Application::PrepareUpdate() {
	frame_count++;
	last_sec_frame_count++;

	delta_time = frame_time.Read();
	dt = (delta_time / 1000.0f) * time_scale;
	if (time_scaleFrames > 0) {
		time_scaleFrames--;
	}
	else if (time_scaleFrames == 0) {
		time_scale = prevTime_scale = 1.f;
		time_scaleFrames = -1;
	}
	frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if(want_to_save == true)
		SavegameNow();


	if (want_to_reload == true) {
		ReloadNow();
	}

	if (want_to_load == true)
		LoadGameNow();

	if (last_sec_frame_time.Read() > 1000) {
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	avg_fps = float(frame_count) / ten_sec_timer.ReadSec();
	ten_sec_timer.ReadSec() >= 10 ? ten_sec_timer.Start(), frame_count = 0 : ' '; //This is questionable
	seconds_since_startup = startup_time.ReadSec();
	last_frame_ms = frame_time.Read();
	frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i  Time since startup: %.3f Frame Count: %lu ",
		avg_fps, last_frame_ms, frames_on_last_update, seconds_since_startup, frame_count);
	win->SetTitle(title);



	delay_time.Start();
	int delay_ms = (1000 / framerate_cap) - last_frame_ms;
	if (delay_ms > 0)
		SDL_Delay(delay_ms);

	uint wait_ms = delay_time.Read();
	//LOG("Expected frame delay: %d, Actual frame delay: %d", delay_ms, wait_ms);
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
	Module* pModule = NULL;

	for(ModuleList::iterator item = modules.begin(); item != modules.end() && ret == true; item++)
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

	for(item = modules.begin(); item != modules.end() && ret == true; item++)
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
	BROFILER_CATEGORY("App_CleanUp", Profiler::Color::Violet);

	xmlDocument	config_file;
	xmlNode		config;
	xmlNode		app_config;

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
		xmlNode node = config.child((*item)->name.c_str());
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

	xmlDocument data;
	xmlNode root;

	pugi::xml_parse_result result = data.load_file((char*)load_game.c_str());

	if(result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.c_str());

		root = data.child("game_state");

		
		ret = true;
		ModuleList::iterator item;

		for(item = modules.begin(); item != modules.end() && ret == true; item++)
		{
			xmlNode node = root.child((*item)->name.c_str());
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
	xmlDocument data;
	xmlNode root;
	
	root = data.append_child("game_state");
	
	ModuleList::const_iterator item = modules.begin();

	for(; item != modules.end() && ret == true; item++)
	{
		xmlNode node = root.append_child((*item)->name.c_str());
		ret = (*item)->Save(node);
		item++;
	}

	if(ret == true)
	{
		data.save_file((char*)save_game.c_str(),"\t");
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", (*item != nullptr) ? (*item)->name.c_str() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}

bool Application::ReloadNow() {

	xmlDocument	config_file;
	xmlNode		config;
	xmlNode		app_config;

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
		//ret = textures->CleanUp(config.child(textures->name.c_str()));
		//pathfinding->CleanUp(config.child(pathfinding->name.GetString())) &
		//map->CleanUp(config.child(map->name.GetString())) &
		//scene->CleanUp(config.child(scene->name.GetString())) &
		//entities->CleanUp(config.child(entities->name.GetString())) &
		//audio->CleanUp(config.child(audio->name.GetString())) &
		//font->CleanUp(config.child(font->name.GetString())) &
		//gui->CleanUp(config.child(gui->name.GetString()));

	if (ret)
		//ret = textures->Awake(config.child(textures->name.c_str())) &
		//map->Awake(config.child(map->name.GetString())) &
		//audio->Awake(config.child(audio->name.c_str()));
		//scene->Awake(config.child(scene->name.GetString())) &
		//entities->Awake(config.child(entities->name.GetString())) &
		//font->Awake(config.child(font->name.GetString())) &
		//gui->Awake(config.child(gui->name.GetString()));

	if (ret)
		ret = textures->Start() &&
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

void Application::SetTimeScale(float ts, int frameNumber)
{
	prevTime_scale = time_scale;
	time_scale = ts;
	time_scaleFrames = frameNumber;
}

void Application::Quit()
{
	want_to_quit = true;
}

void Application::Reload() {
	want_to_reload = true;
}

void Application::PauseGame(bool pause) {
	SetTimeScale(pause ? 0.f : 1.f);
	entities->PauseEntities(pause);

}