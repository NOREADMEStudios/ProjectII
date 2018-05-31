#include "ModuleCinematics.h"



ModuleCinematics::ModuleCinematics()
{
}


ModuleCinematics::~ModuleCinematics()
{
}

bool ModuleCinematics::Start()
{
	return false;
}

bool ModuleCinematics::PreUpdate()
{
	return false;
}

bool ModuleCinematics::Update(float dt)
{
	return false;
}

bool ModuleCinematics::PostUpdate()
{
	return false;
}

bool ModuleCinematics::CleanUp(pugi::xml_node &)
{
	return false;
}

void ModuleCinematics::StartCinematic(Cinematic * c)
{
}

float ModuleCinematics::GetCurrentCinematicTime()
{
	return 0.0f;
}

float ModuleCinematics::GetCurrentCinematicTimeLeft()
{
	return 0.0f;
}

bool ModuleCinematics::PlayCurrentCinematic()
{
	return false;
}
