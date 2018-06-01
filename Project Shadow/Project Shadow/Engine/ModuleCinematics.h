#ifndef __MODULE_CINEMATICS_H__
#define __MODULE_CINEMATICS_H__

#include "Module.h"
#include "Defs.h"
#include "Point.h"

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

	bool IsPlaying() { return currentCinematic != nullptr; }

private:
	bool PlayCurrentCinematic();
	Cinematic* currentCinematic = nullptr;
};

#endif