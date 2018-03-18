// ----------------------------------------------------
// Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __MODULE_H__
#define __MODULE_H__

#include <string>
#include "../PugiXml/src/pugixml.hpp"

struct Collider;

class Application;

class UI_Elem;

enum ButtonEvent;

class Module
{
public:

	Module() : active(false)
	{}

	virtual void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void OnCollision(Collider* c1, Collider* c2)
	{

	}

	virtual bool OnEventChange(UI_Elem* elem, ButtonEvent event)  { return true; } //?

public:

	std::string	name;
	bool		active;

};

#endif // __MODULE_H__