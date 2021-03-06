#include "ModuleTransition.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "App.h"


ModuleTransition::ModuleTransition()
{
	name = "transition";
}


ModuleTransition::~ModuleTransition()
{
}

ModuleTransition::Transition ModuleTransition::GetTransitionType()
{
	return type;
}

bool ModuleTransition::MakeTransition(Callback _cb, Transition _type, float time)
{
	//App->SetTimeScale(0.f, 60);
	completed = false;
	callback = _cb;
	transitionTimeTotal = time;
	transitionTimeCurrent = 0.0f;
	type = _type;
	uint w = DEFAULT_RESOLUTION_X, h = DEFAULT_RESOLUTION_Y;
	//App->win->GetWindowSize(w, h);
	dstRect.x = 0;
	dstRect.y = 0;
	switch (type)
	{
	case ModuleTransition::NONE:
		break;
	case ModuleTransition::FADE_TO_BLACK:
		fadeState = FADING_OUT;
		break;
	case ModuleTransition::SCROLL_RIGHT:
		speed = (int)(w / time);
	case ModuleTransition::SCROLL_LEFT:
		scrollState = SNAPSHOT;
		speed = -(int)(w / time);
		break;
	case ModuleTransition::LOADING_SCREEN:
		break;
	default:
		break;
	}
	return false;
}

void ModuleTransition::FadeToBlack()
{
	fadeAlpha = MIN(1.0f, transitionTimeCurrent / transitionTimeTotal);
	switch (fadeState)
	{
	case ModuleTransition::FADING_OUT:
		if (transitionTimeCurrent > transitionTimeTotal) {
			fadeState = FADING_IN;
			transitionTimeCurrent = 0.0f;
			if (callback != nullptr) {
				callback(0);
				callback = nullptr;
			}
		}
		break;
	case ModuleTransition::FADING_IN:
		fadeAlpha = 1.0f - fadeAlpha;
		if (transitionTimeCurrent > transitionTimeTotal) {
			fadeAlpha = 0;
			fadeState = NOT_FADING;
			completed = true;
		}
		break;
	default:
		break;
	}
	App->render->DrawQuad(dstRect.toSDL_Rect(), 0, 0, 0, CLAMP((fadeAlpha * 255.0f), 0, 255), 0.0f, true, false);
}

void ModuleTransition::Scrolling()
{
	if (scrollTexture != nullptr) {
		App->render->BlitGui(scrollTexture, dstRect.x, dstRect.y, nullptr, false);
	}
	switch (scrollState)
	{
	case ModuleTransition::SNAPSHOT:
		if (callback != nullptr) {
			callback(0);
			callback = nullptr;
		}
		scrollTexture = App->render->Snapshot();
		scrollState = SCROLLING;
		transitionTimeCurrent = 0.0f;
		break;
	case ModuleTransition::SCROLLING:
		if (dstRect.x > DEFAULT_RESOLUTION_X || dstRect.x < -DEFAULT_RESOLUTION_X) {
			completed = true;
			scrollState = NOT_SCROLLING;
			SDL_DestroyTexture(scrollTexture);
			scrollTexture = nullptr;
		}
		break;
	default:
		break;
	}
}

void ModuleTransition::LoadScreen()
{
}

bool ModuleTransition::Awake(pugi::xml_node &)
{
	return true;
}

bool ModuleTransition::Start()
{
	completed = false;
	uint w = DEFAULT_RESOLUTION_X, h = DEFAULT_RESOLUTION_Y;
	//App->win->GetWindowSize(w, h);
	fadeAlpha = 0;
	speed = 0;
	dstRect = { 0, 0, (int)w, (int)h };
	int ret = SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	if (ret != 0)
	{
		LOG("Could not create transition surface: %s", SDL_GetError());
		return false;
	}
	return true;
}

bool ModuleTransition::PostUpdate()
{
	if (!completed)
	{
		switch (type)
		{
		case ModuleTransition::NONE:
			completed = true;
			if (callback != nullptr) {
				callback(0);
				callback = nullptr;
			}
			break;
		case ModuleTransition::FADE_TO_BLACK:
			FadeToBlack();
			break;
		case ModuleTransition::SCROLL_RIGHT:
		case ModuleTransition::SCROLL_LEFT:
			Scrolling();
			break;
		case ModuleTransition::LOADING_SCREEN:
			break;
		default:
			break;
		}
	}
	return true;
}

bool ModuleTransition::Update(float dt)
{
	bool ret = true;

	transitionTimeCurrent += dt;

	if (!completed)
	{
		switch (type)
		{
		case ModuleTransition::SCROLL_RIGHT:
		case ModuleTransition::SCROLL_LEFT:
			dstRect.x += speed * dt;
			break;
		default:
			break;
		}
	}

	return ret;
}


bool ModuleTransition::CleanUp(pugi::xml_node &)
{
	if (fadeTexture != nullptr)
		SDL_DestroyTexture(fadeTexture);
	if (scrollTexture != nullptr)
		SDL_DestroyTexture(scrollTexture);
	fadeTexture = nullptr;
	scrollTexture = nullptr;
	return true;
}
