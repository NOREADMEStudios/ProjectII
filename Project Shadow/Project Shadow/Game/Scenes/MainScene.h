#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"
#include "..\..\PugiXml\src\pugixml.hpp"

class MainScene :
	public Scene
{
public:

	
	Entity* e,* e2, * e3, *e4;
	SDL_Texture* t;

	MainScene();
	virtual ~MainScene();

	bool Awake(pugi::xml_node&) override { return true; };

	bool Start() override;

	bool PreUpdate() override { return true; }

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	bool Load(pugi::xml_node&) override { return true; };

	bool Save(pugi::xml_node&) const override { return true; };


};

#endif