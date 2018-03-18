#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "Module.h"


struct SDL_Texture;
struct SDL_Surface;

class ModuleTextures : public Module
{
public:

	ModuleTextures();

	// Destructor
	virtual ~ModuleTextures();

	// Called before render is available
	bool Awake(pugi::xml_node&) override;

	// Called before the first frame
	bool Start() override;

	// Called before quitting
	bool CleanUp(pugi::xml_node&) override;

	// Load Texture
	SDL_Texture* const	Load(const char* path);
	bool				UnLoad(SDL_Texture* texture);
	SDL_Texture* const	LoadSurface(SDL_Surface* surface);
	void				GetSize(const SDL_Texture* texture, uint& width, uint& height) const;

public:

	std::list<SDL_Texture*>	textures;
};


#endif // __TEXTURES_H__