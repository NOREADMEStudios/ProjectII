#ifndef __INTERFACE_ELEMENT_H__
#define __INTERFACE_ELEMENT_H__

#include "..\Defs.h"
#include "..\Log.h"
#include "..\App.h"
#include "..\ModuleRender.h"
#include "..\ModuleTextures.h"
#include "..\Rect.h"
#include "..\Point.h"

class InterfaceElement
{
public:
	InterfaceElement();
	~InterfaceElement();
	enum Interfacetype
	{
		NONE = -1,
		BUTTON,
		LABEL,
		SPRITE,
		TEXT_INPUT,
		CHECK_BUTTON,
		WINDOW,
		SLIDER,
		HEALTHBAR
	};

	enum Directions {
		UP,
		RIGHT,
		DOWN,
		LEFT,
		AMOUNT
	};


	bool Enable(bool enable);
	bool isEnabled() const;
	bool isCulled()const;
	virtual bool Start();
	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();

	virtual SDL_Rect getRect() const;
	virtual void SetContentRect(int x_margin = INT_MAX, int y_margin = INT_MAX, int x_margin_2 = INT_MAX, int y_margin_2 = INT_MAX);
	SDL_Rect GetContentRect() const;
	virtual int getPositionX() const;
	virtual int getPositionY() const;
	virtual iPoint getPosition() const;
	virtual void setPosition(int x, int y);
	virtual void setPositionX(int x);
	virtual void setPositionY(int y);
	virtual void setScale(float scale);
	virtual float getScale() const;
	Interfacetype getType() const;

	void SetAnchor(float x, float y);
	void GetAnchor(float& x, float& y) const;
	float GetAnchorX() const;
	float GetAnchorY() const;

	void SetParent(InterfaceElement* parent);

	InterfaceElement* getNextChild(InterfaceElement*);
	InterfaceElement* getPrevChild(InterfaceElement*);
	InterfaceElement* getNextSibling();
	InterfaceElement* getPrevSibling();

	void SetRelation(InterfaceElement* elem, Directions dir, bool assignOther = true);
	InterfaceElement* GetRelativeElement(Directions dir);

	bool isInteractuable();

	void ComputeRects();

	SDL_Texture*		tex = nullptr;
	Interfacetype		type = NONE;
	SDL_Rect			rect,
						content_rect,
						content_rect_margins,
						result_rect;
	iPoint				rel_pos,
						abs_pos,
						initial_pos;
	float				scale = 1.0f;
	bool				in_focus = false,
						culled = false,
						next_frame_enabled = true,
						enabled = true;

protected:

	void DebugDraw();

	InterfaceElement* AddElement(InterfaceElement* elem);

	LIST(InterfaceElement*)	children;
	InterfaceElement*		parent = nullptr;
	InterfaceElement*		relations[Directions::AMOUNT];

	fPoint					anchor_point = { 0.5f, 0.5f };
	SDL_Rect*				currentAnim = nullptr;
	bool					interactuable = false;
};
#endif