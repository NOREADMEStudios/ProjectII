#include "Defs.h"
#include "Log.h"
#include "ModuleAudio.h"

#include "../SDL/include/SDL.h"
#include "../SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

#define DEFAULT_VOLUME_LEVEL MIX_MAX_VOLUME/2

ModuleAudio::ModuleAudio() : Module()
{
	music = NULL;
	name = "audio";

	currentfxvolume = DEFAULT_VOLUME_LEVEL;
	currentmusicvolume = DEFAULT_VOLUME_LEVEL;
}

ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Awake(pugi::xml_node& config)
{
	namespace filesystem = std::experimental::filesystem;
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	assetsPath = ASSETS_ROOT;
	assetsPath.append(config.attribute("folder").as_string());

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the OGG audio formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags) {
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	SetFxVolume(config.attribute("volumeFX").as_float());
	SetMusicVolume(config.attribute("volumeBGM").as_float());

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp(pugi::xml_node&)
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	
	for(int i = 0;i < fx.size(); i++)
		Mix_FreeChunk(fx[i]);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Update
bool ModuleAudio::Update(float dt)
{
	if (fading)
	{
		Dvolume = currentmusicvolume / (fade_time / dt);
		if (Dvolume < 1)
			Dvolume = 1;
		if (Dvolume > currentmusicvolume / fade_time)
			Dvolume > currentmusicvolume / fade_time;
		if (newMusic != nullptr) // It means we are still fading out the initial music
		{
			fadingOut();
		}
		else
		{
			fadingIn();
		}
	}

	return true;
}

// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if (!active)
		return false;

	if (music == NULL)
	{
		music = Mix_LoadMUS(path);
		if (Mix_PlayMusic(music, -1) < 0)
		{
			LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
			ret = false;
		}
	}
	else
		newMusic = Mix_LoadMUS(path);

	if (fade_time > 0)
	{
		fading = true;
		this->fade_time = fade_time;
		LOG("Started Fading %s", path);
	}
	else
	{
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = newMusic;
		if (Mix_PlayMusic(music, -1) < 0)
		{
			LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
			ret = false;
		}
		newMusic = nullptr;
		LOG("Successfully playing %s", path);
	}

	return ret;
}

// Load WAV
unsigned int ModuleAudio::LoadFx(const char* path){
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);


	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		Mix_VolumeChunk(chunk, currentfxvolume);
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play WAV
bool ModuleAudio::PlayFx(unsigned int id, int repeat){
	bool ret = false;

	if(!active)
		return false;

	if(id > 0 && id <= fx.size())
	{
		Mix_PlayChannel(-1, fx[id - 1], repeat);
	}

	return ret;
}

bool ModuleAudio::CleanFx(){
	fx.clear();
	return true;
}
void ModuleAudio::SetMusicVolume(float volume) {

	if (volume > MIX_MAX_VOLUME) { volume = MIX_MAX_VOLUME; }

	Mix_VolumeMusic(volume);
	currentmusicvolume = volume;
}

void ModuleAudio::SetFxVolume(float volume) {

	if (volume > MIX_MAX_VOLUME) { volume = MIX_MAX_VOLUME; }

	
	for (int i = 0; i < fx.size(); i++) {
		Mix_VolumeChunk(fx[i], volume);	
	}
	currentfxvolume = volume;
}

float ModuleAudio::GetFxVolume() const {

	return currentfxvolume;
}

float ModuleAudio::GetMusicVolume() const {

	return Mix_VolumeMusic(-1);
}

bool ModuleAudio::Load(pugi::xml_node& data) {

	pugi::xml_node music = data.child("music");

	float volume = music.attribute("volume").as_float();
	SetMusicVolume(volume);

	pugi::xml_node fx = data.child("fx");

	volume = fx.attribute("volume").as_float();
	SetFxVolume(volume);
	
	return true;
}
bool ModuleAudio::Save(pugi::xml_node& data)const {

	pugi::xml_node music = data.append_child("music");
	music.append_attribute("volume") = GetMusicVolume();

	pugi::xml_node fx = data.append_child("fx");
	fx.append_attribute("volume") = GetFxVolume();

	return true;
}

void ModuleAudio::fadingIn()
{
	if (music != NULL)
	{
		int newVolume = GetMusicVolume() + Dvolume;
		if (newVolume > currentmusicvolume)
			newVolume = currentmusicvolume;
		Mix_VolumeMusic(newVolume);
		if (newVolume == currentmusicvolume)
		{
			fading = false;
		}
	}
}

void ModuleAudio::fadingOut()
{
	if (music != NULL)
	{
		int newVolume = GetMusicVolume() - Dvolume;
		if (newVolume < 0)
			newVolume = 0;
		Mix_VolumeMusic(newVolume);
		if (newVolume == 0)
		{
			Mix_FreeMusic(music);
			music = newMusic;
			newMusic = nullptr;
			if (Mix_PlayMusic(music, -1) < 0)
				LOG("Cannot play in music. Mix_GetError(): %s", Mix_GetError());
		}
	}
}