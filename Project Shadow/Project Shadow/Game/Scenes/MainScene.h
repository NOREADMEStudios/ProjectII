#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"
#include "..\..\PugiXml\src\pugixml.hpp"

class Label;

class MainScene :
	public Scene
{
public:

	
	Entity* e = nullptr,* e2 = nullptr, * e3 = nullptr, *e4 = nullptr;
	int		totalRounds,
			currentRound,
			wonRounds[2];
	Label	*roundsLabel = nullptr,
			*winnerLabel = nullptr;
	bool	combatEndControlBool = false;

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