#ifndef __CINEMATIC_H__
#define __CINEMATIC_H__
#include "Point.h"
#include <vector>

struct CinematicFrame {
	iPoint cameraPosition;
	float cameraScale = 1.f;
};

struct CinematicTransition {
	CinematicFrame * initialFrame = nullptr,
		* endFrame = nullptr;
	float duration = 0.f;
	bool interpolatePosition = true,
		interpolateScale = true;
};

class Cinematic
{
public:
	Cinematic();
	virtual ~Cinematic();

	void AddKeyFrame(CinematicFrame*, float duration, bool interpolatePosition = true, bool interpolateScale = true);

private:
	std::vector<CinematicTransition> keyFrames;
};

#endif