#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Scene.h"
#include "Module.h"

#include <list>


class ModuleSceneManager : public Module
{
public:
	ModuleSceneManager();
	~ModuleSceneManager();

	bool Awake(pugi::xml_node&) override;

	bool Start()override;

	bool PreUpdate()override;

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void LoadScene(Scene* scene);
	Scene* ChangeScene(Scene* scene_to_change);
	void UnloadScene(Scene* scene);

private:

	std::list<Scene> scenes;
	Scene* currentScene;

};

#endif // !__ENTITYMANAGER_H__
