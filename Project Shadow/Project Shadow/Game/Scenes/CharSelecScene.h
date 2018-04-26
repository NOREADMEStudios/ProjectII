#ifndef __SELECSCENE_H__
#define __SELECSCENE_H__

#include "..\..\Engine\Scene.h"
#include "..\..\Engine\ModuleCollision.h"

#include <list>

class Button;
class Sprite;
class Label;
class InterfaceElement;

class CharSelecScene :
	public Scene
{
public:

	struct Selection {

		std::list<SDL_Rect> rectTextures;

	};


	int controllersNum;

	CharSelecScene();
	virtual ~CharSelecScene();

	bool Awake(pugi::xml_node&) override { return true; };

	bool Start() override;

	bool PreUpdate() override { return true; }

	bool Update(float dt) override;

	bool PostUpdate() override { return true; }

	bool CleanUp() override;

	bool Load(pugi::xml_node&) override { return true; };

	bool Save(pugi::xml_node&) const override { return true; };

private:
	void LoadSceneUI();


	void SetControllerFocus();
	void ManageDisplacementFocus();
	void ChooseFocus();
	


};

#endif