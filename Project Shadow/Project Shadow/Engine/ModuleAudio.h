#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"
#include <vector>
#include "../SDL_mixer/include/SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class ModuleAudio : public Module
{
public:

	ModuleAudio();

	// Destructor
	virtual ~ModuleAudio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	bool CleanFx();

	bool Load(pugi::xml_node&)override;
	bool Save(pugi::xml_node&)const override;

	void SetMusicVolume(float volume);
	float GetMusicVolume()const;

	void SetFxVolume(float volume);
	float GetFxVolume()const;



private:
	float currentmusicvolume;
	float currentfxvolume;


	_Mix_Music*			music;
	std::vector<Mix_Chunk*>	fx;
};

#endif // __AUDIO_H__