#ifndef __MODULE_CINEMATICS_H__
#define __MODULE_CINEMATICS_H__

#include "Module.h"
#include "Defs.h"
#include "Point.h"
#include <dshow.h>
#include <Vfw.h>
#pragma comment( lib, "Vfw32.lib" )


class Cinematic;

class ModuleCinematics :
	public Module
{
public:
	ModuleCinematics();
	virtual ~ModuleCinematics();

	bool Awake(pugi::xml_node&) override;

	bool Start() override;

	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;

	bool CleanUp(pugi::xml_node&) override;

	void StartCinematic(Cinematic*);

	float GetCurrentCinematicTime();
	float GetCurrentCinematicTimeLeft();

	iPoint GetInitialCameraPosition();

	bool PlayVideo(const char* path);
	void CloseVideo();
	void GetNextFrame();

	bool IsPlaying() { return (currentCinematic != nullptr) && !videoPlaying; }

private:
	bool PlayCurrentCinematic();
	Cinematic* currentCinematic = nullptr;
	std::string assetsPath = "";

	bool	videoPlaying = false;
	long	videoWidth			= 0,
			videoHeight			= 0,
			videoLastFrame		= 0,
			videoTimeLength		= 0,
			videoCurrentFrame	= 0;

	PAVISTREAM videoStream = nullptr;
	PGETFRAME videoFrame = nullptr;
};

#endif