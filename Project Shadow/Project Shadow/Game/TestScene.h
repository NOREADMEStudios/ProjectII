#ifndef __SCENE_TEST_H__
#define __SCENE_TEST_H__

#include "..\Engine\Scene.h"
#include "..\Engine\ModuleCollision.h"

class TestScene :
	public Scene
{
public:

	Collider c, c2;
	Entity e, e2;
	float angle = 0;

	TestScene();
	virtual ~TestScene();

	bool Awake(pugi::xml_node&) override { return true; };

	bool Start() override;

	bool PreUpdate() override { return true; }

	bool Update(float dt) override;

	bool PostUpdate() override { return true; }

	bool CleanUp(pugi::xml_node&) override { return true; };

	bool Load(pugi::xml_node&) override { return true; };

	bool Save(pugi::xml_node&) const override { return true; };
};

#endif