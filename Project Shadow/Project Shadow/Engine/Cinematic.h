#ifndef __CINEMATIC_H__
#define __CINEMATIC_H__
#include "Point.h"
#include <vector>
#include "Defs.h"

struct CinematicFrame {
	iPoint cameraPosition;
	float cameraScale = 1.f;
	Callback OnEnd = nullptr;

	CinematicFrame(){}
	CinematicFrame(iPoint pos, float scale) : cameraPosition(pos), cameraScale(scale){}
	~CinematicFrame(){}

};

struct CinematicTransition {
	CinematicFrame * initialFrame = nullptr,
		* endFrame = nullptr;

	float duration = 0.f,
		transcurredTime = 0.f;

	bool interpolatePosition = true,
		interpolateScale = true;

	CinematicTransition(){}
	CinematicTransition(CinematicFrame*start, CinematicFrame*end, float duration) : initialFrame(start), endFrame(end), duration(duration){}
	~CinematicTransition(){
		Utils::Release(initialFrame);
		Utils::Release(endFrame);
	}
};

class Cinematic
{
public:
	Cinematic();
	virtual ~Cinematic();

	bool Update(float dt);

	void AddKeyFrame(CinematicFrame* keyFrame, float duration, bool interpolatePosition = true, bool interpolateScale = true, Callback cb = nullptr);
	void SetOnCompleteCallback(Callback);
	void SetOnProgressCallback(Callback);

	CinematicTransition* GetCurrentTransition();

	float GetDuration() { return duration; }
	float GetDurationLeft() { return duration - currentRunningTime; }

private:
	std::vector<CinematicTransition*> transitions;
	size_t currentTransition = 0;

	Callback OnProgressUpdate = nullptr;
	Callback OnCinematicEnd = nullptr;

	float currentRunningTime = 0.f;
	float duration = 0.f;
};

#endif