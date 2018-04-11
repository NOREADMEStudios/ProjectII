#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "Defs.h"
#include "../PugiXml/src/pugixml.hpp"
#include "Point.h"
#include "Module.h"
#include <list>
#include <filesystem>
//#include "Collision.h"

namespace filesystem = std::experimental::filesystem;
struct SDL_Texture;

struct MapLayer {
	uint*			tiles = nullptr;
	std::string		name = "";
	uint			width = 0;
	uint			height = 0;
	uint			size = 0;
	float			parallax_speed = 1.0f;

	~MapLayer() {
		free(tiles);
	}

};

// ----------------------------------------------------

	
//iPoint getWorldPositionFromMap(int x, int y, uint tile_width);


// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name = "";
	int					firstgid = 0;
	int					tileCount = 0;
	int					margin = 0;
	int					spacing = 0;
	int					tile_width = 0;
	int					tile_height = 0;
	SDL_Texture*		texture = nullptr;
	int					tex_width = 0;
	int					tex_height = 0;
	int					num_tiles_width = 0;
	int					num_tiles_height = 0;
	int					offset_x = 0;
	int					offset_y = 0;

	~TileSet();
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// ----------------------------------------------------
struct MapData
{
	int					width = 0;
	int					height = 0;
	int					tile_width = 0;
	int					tile_height = 0;
	SDL_Color			background_color;
	MapTypes			type = MAPTYPE_UNKNOWN;
	std::list<TileSet*>	tilesets;
	std::list<MapLayer*>	layers;

	~MapData() {

		for (std::list<TileSet*>::iterator it = tilesets.begin(); it != tilesets.end(); it++) {
			Utils::Release(*it);
		}
		tilesets.clear();

		for (std::list<MapLayer*>::iterator it = layers.begin(); it != layers.end(); it++) {
			Utils::Release(*it);
		}
		layers.clear();
	}
};


// ----------------------------------------------------
class ModuleMap : public Module
{
public:

	ModuleMap();

	// Destructor
	virtual ~ModuleMap();

	// Called before render is available
	bool Awake(pugi::xml_node& conf) override;

	// Called each frame
	void Draw();

	// Called before quitting
	bool CleanUp(pugi::xml_node&) override;

	// Load new map
	bool Load(const char* path);


	iPoint MapToWorld(int x, int y) const;
	iPoint MapToWorld(const iPoint p) const;

	iPoint WorldToMap(int x, int y) const;
	iPoint WorldToMap(const iPoint p) const;
	int GetMapWidth();
	int GetXTiles();
	int GetMapHeight();

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet& set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet& set);
	bool LoadLayer(pugi::xml_node& node, MapLayer& layer);

private:

	pugi::xml_document	map_file;
	bool				map_loaded = false;
	MapData				data;
	std::string			folder = "";
};

#endif // __j1MAP_H__