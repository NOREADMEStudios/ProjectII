#include "ModuleCinematics.h"
#include "Cinematic.h"
#include "ModuleInput.h"


ModuleCinematics::ModuleCinematics()
{
}


ModuleCinematics::~ModuleCinematics()
{
}

bool ModuleCinematics::Awake(pugi::xml_node &)
{
	return true;
}

bool ModuleCinematics::Start()
{
	return true;
}

bool ModuleCinematics::PreUpdate()
{
	if (currentCinematic != nullptr)
		App->input->BlockAllInput();

	return true;
}

bool ModuleCinematics::Update(float dt)
{
	if (currentCinematic != nullptr) {
		if (!currentCinematic->Update(dt)) {
			Utils::Release(currentCinematic);
			currentCinematic = nullptr;
		}
	}
	return true;
}

bool ModuleCinematics::PostUpdate()
{
	return true;
}

bool ModuleCinematics::CleanUp(pugi::xml_node &)
{
	if (currentCinematic != nullptr) {
		Utils::Release(currentCinematic);
		currentCinematic = nullptr;
	}
	return false;
}

void ModuleCinematics::StartCinematic(Cinematic * c)
{
	currentCinematic = c;
}

float ModuleCinematics::GetCurrentCinematicTime()
{
	if (currentCinematic != nullptr)
		return currentCinematic->GetDuration();
	else return 0.f;
}

float ModuleCinematics::GetCurrentCinematicTimeLeft()
{
	if (currentCinematic != nullptr)
		return currentCinematic->GetDurationLeft();
	else return 0.f;
}

iPoint ModuleCinematics::GetInitialCameraPosition()
{
	if (currentCinematic != nullptr)
		return currentCinematic->GetCurrentTransition()->initialFrame->cameraPosition;
	else return iPoint(-1, -1);
}

bool ModuleCinematics::PlayCurrentCinematic() //No use really
{
	return false;
}
