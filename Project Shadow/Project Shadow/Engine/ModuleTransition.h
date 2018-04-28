#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#include "Module.h"
#include "Defs.h"
#include "Rect.h"

struct SDL_Texture;

class ModuleTransition :
	public Module
{
public:
	enum Transition {
		NONE = -1,
		FADE_TO_BLACK,
		SCROLL_RIGHT,
		SCROLL_LEFT,
		LOADING_SCREEN
	};

	ModuleTransition();
	virtual ~ModuleTransition();

	// Called before render is available
	virtual bool Awake(pugi::xml_node&);

	// Called before the first frame
	virtual bool Start();

	// Called each loop iteration
	virtual bool PostUpdate();

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called before quitting
	virtual bool CleanUp(pugi::xml_node&);

	Transition GetTransitionType();
	bool MakeTransition(Callback _cb, Transition _type, float time);

private:
	void FadeToBlack();
	void Scrolling();
	void LoadScreen();

	enum FadeState {
		NOT_FADING = -1,
		FADING_OUT,
		FADING_IN
	} fadeState = NOT_FADING;

	enum ScrollState {
		NOT_SCROLLING = -1,
		SNAPSHOT,
		SCROLLING
	} scrollState = NOT_SCROLLING;

	SDL_Texture *	fadeTexture = nullptr,
				*	scrollTexture = nullptr;
	float			transitionTimeTotal	= 0.f,
					transitionTimeCurrent = 0.f,
					fadeAlpha = 0.f,
					speed = 0.f;
	bool			completed = false;
	Transition		type = NONE;
	Callback		callback = nullptr;
	iRect			dstRect;
};

#endif