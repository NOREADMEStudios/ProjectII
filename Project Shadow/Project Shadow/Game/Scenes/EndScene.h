#ifndef __ENDSCENE_H__
#define __ENDSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"

class Button;
class Label;
class EndScene :
	public Scene
{
public:

	Button * mainMenuButton;
	Label * mainLabel;
	Label * winnerLabel;
	SDL_Texture * atlas;

	EndScene();
	virtual ~EndScene();

	bool Awake(pugi::xml_node&) override { return true; };

	bool Start() override;

	bool PreUpdate() override { return true; }

	bool Update(float dt) override;

	bool PostUpdate() override { return true; }

	bool CleanUp() override;

	bool Load(pugi::xml_node&) override { return true; };

	bool Save(pugi::xml_node&) const override { return true; };

};

#endif