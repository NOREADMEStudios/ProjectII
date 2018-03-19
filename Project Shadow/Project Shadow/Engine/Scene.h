#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "Module.h"


class Scene : public Module
{
public:
	Scene();
	~Scene();


	bool Awake(pugi::xml_node&) override;

	bool Start()override;

	bool PreUpdate()override;

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	bool loaded = false;
};
#endif



