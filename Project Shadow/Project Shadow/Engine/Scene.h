#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"



struct SDL_Texture;
class Scene : public Module
{
public:
	Scene();
	~Scene();


	bool Awake(pugi::xml_node&) override;

	bool Start()override { return true; }

	bool PreUpdate()override { return true; }

	bool Update(float dt)override;

	bool PostUpdate()override { return true; }

	virtual bool CleanUp();

	bool Load(pugi::xml_node&) override { return true; };
	bool Save(pugi::xml_node&) const override { return true; };

	bool LoadBackground(std::string path);
	bool DrawBackground();
	bool UnLoadBackground();


	bool loaded = false;
	bool paused = false;

	bool enableMouse = false;

	SDL_Texture* background;

};
#endif



