#ifndef __ENDSCENE_H__
#define __ENDSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"

class EndScene :
	public Scene
{
public:


	EndScene();
	virtual ~EndScene();

	bool Awake(pugi::xml_node&) override { return true; };

	bool Start() override { return true; };

	bool PreUpdate() override { return true; }

	bool Update(float dt) override { return true; };

	bool PostUpdate() override { return true; }

	bool CleanUp(pugi::xml_node&) override { return true; };

	bool Load(pugi::xml_node&) override { return true; };

	bool Save(pugi::xml_node&) const override { return true; };

};

#endif