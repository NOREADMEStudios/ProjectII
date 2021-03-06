#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Scene.h"
#include "Module.h"

#include <list>

class IntroScene;
class CharacterSelecScene;
class ItemSelecScene;
class MainScene;
class EndScene;


enum GameMode {
	NOMODE,
	ONEvsONE,
	TWOvsTWO

};


class ModuleSceneManager : public Module
{
public:
	ModuleSceneManager();
	~ModuleSceneManager();

	bool Awake(pugi::xml_node&) override;

	bool Start()override;

	bool PreUpdate()override { return true; }

	bool Update(float dt)override;

	bool PostUpdate()override;

	bool CleanUp(pugi::xml_node&)override;

	bool Load(pugi::xml_node&) { return true; }
	bool Save(pugi::xml_node&) const { return true; }

	void AddScene(Scene* scene);
	void LoadScene(Scene* scene);
	void ChangeScene(Scene* scene_to_change);
	void UnloadScene(Scene* scene);
	void EnableDebugLab();

	IntroScene* introSc;
	CharacterSelecScene* characterSc;
	ItemSelecScene* itemSc;
	MainScene* mainSc;
	EndScene* endSc;
		
	GameMode gameMode = NOMODE;

private:

	std::list<Scene> scenes;
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;

};

#endif // !__SCENEMANAGER_H__
