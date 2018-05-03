#ifndef __WIZARD_H__
#define __WIZARD_H__

#include "Module.h"
#include "Character.h"
#include "ModuleEntityManager.h"
#include "Animation.h"
#include "Timer.h"




class Wizard : public Character
{
public:
	Wizard();
	~Wizard();

	bool Awake(pugi::xml_node&) override;

	bool WizardStart();

	bool PreUpdate()override;

	bool WizardUpdate(float dt);

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&)override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	void UpdateSpecStates();


private:
	bool buffed = false;

};
#endif