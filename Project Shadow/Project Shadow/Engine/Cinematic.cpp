#include "Cinematic.h"
#include "Defs.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "App.h"


Cinematic::Cinematic()
{
}


Cinematic::~Cinematic()
{
}

bool Cinematic::Update(float dt)
{
	bool ret = true;
	CinematicTransition* ct = transitions[currentTransition];
	float speed = dt / ct->duration;
	currentRunningTime += dt;
	float scale = App->win->GetScale();

	if (ct->transcurredTime <= ct->duration) {
		ct->transcurredTime += dt;

		if (ct->interpolatePosition) {
			App->render->camera.x = Utils::Interpolate((float)App->render->camera.x, (float)ct->endFrame->cameraPosition.x, speed) * scale;
			App->render->camera.y = Utils::Interpolate((float)App->render->camera.y, (float)ct->endFrame->cameraPosition.y, speed) * scale;
		}
		else {
			App->render->camera.x = ct->endFrame->cameraPosition.x;
			App->render->camera.y = ct->endFrame->cameraPosition.y;
		}

		if (ct->interpolateScale)
			App->win->SetScale(Utils::Interpolate(scale, ct->endFrame->cameraScale, speed));
		else App->win->SetScale(ct->endFrame->cameraScale);
	}
	else {
		if (ct->endFrame->OnEnd != nullptr)
			ct->endFrame->OnEnd(0);

		if (++currentTransition >= transitions.size())
			ret = false;
	}

	if (OnProgressUpdate != nullptr) {
		OnProgressUpdate(3, (float)currentTransition, currentRunningTime, duration);
	}

	return ret;
}

void Cinematic::AddKeyFrame(CinematicFrame * keyFrame, float duration, bool interpolatePosition, bool interpolateScale, Callback cb)
{
	CinematicTransition * ct = new CinematicTransition();
	ct->duration = duration;
	ct->interpolatePosition = interpolatePosition;
	ct->interpolateScale = interpolateScale;
	ct->endFrame = keyFrame;
	keyFrame->OnEnd = cb;
	this->duration += duration;

	if (transitions.size() > 0) {
		ct->initialFrame = transitions[transitions.size() - 1]->endFrame;
	}

	transitions.push_back(ct);
}

void Cinematic::SetOnCompleteCallback(Callback cb)
{
	OnCinematicEnd = cb;
}

void Cinematic::SetOnProgressCallback(Callback cb)
{
	OnProgressUpdate = cb;
}

CinematicTransition * Cinematic::GetCurrentTransition()
{
	return transitions[currentTransition];
}
