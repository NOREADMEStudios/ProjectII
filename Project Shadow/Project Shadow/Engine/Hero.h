#ifndef __HERO_H__
#define __HERO_H__

#include "Module.h"
#include "Character.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Timer.h"

#define COMBO_MARGIN 0.5

class Hero : public Character
{
public:
	Hero();
	~Hero();


	bool Awake(pugi::xml_node&) override;

	bool Start()override;

	bool PreUpdate()override;

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&)override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	void UpdateInputs(float dt);

	void LoadAnimations();

	void RequestState();
	void UpdateState();
	void UpdateCurState(float dt);
	void UpdateAnimation();
	void PushInBuffer(Input state);

	LIST(Input) inputBuffer;

	Timer time_light_attack;
	Timer time_strong_attack;

};
#endif
