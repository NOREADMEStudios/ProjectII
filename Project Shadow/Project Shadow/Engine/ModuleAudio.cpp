#include "Defs.h"
#include "Log.h"
#include "ModuleAudio.h"

#include "../SDL/include/SDL.h"
#include "../SDL_mixer/include/SDL_mixer.h"
_//#pragma comment( lib, "../SDL_mixer/libx86/SDL2_mixer.lib" )

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
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
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

// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
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

	return currentmusicvolume;
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
