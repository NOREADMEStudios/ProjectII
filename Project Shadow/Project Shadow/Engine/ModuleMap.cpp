#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleMap.h"
#include <math.h>
#include <stdlib.h>

#include "..\Brofiler\Brofiler.h"

ModuleMap::ModuleMap() : Module(), map_loaded(false)
{
	name = "map";
}

// Destructor
ModuleMap::~ModuleMap() {}

// Called before render is available
bool ModuleMap::Awake(pugi::xml_node& config) {
	LOG("Loading Map Parser");
	bool ret = true;

	folder = ASSETS_ROOT;
	folder.append(config.attribute("folder").as_string());

	return ret;
}

void ModuleMap::Draw() {

	BROFILER_CATEGORY(__FUNCTION__, Profiler::Color::SlateBlue);

	bool player_blit = false;
	if(map_loaded == false)
		return;

	for (std::list<MapLayer*>::iterator iter = data.layers.begin(); iter != data.layers.end(); iter++) {
		uint tile = 0;
			for (uint y = 0; y < (*iter)->height; y++) {
				for (uint x = 0; x < (*iter)->width; x++) {
					if ((*iter)->tiles[tile] != 0) {
						for (std::list<TileSet*>::reverse_iterator tileset = data.tilesets.rbegin(); tileset != data.tilesets.rend(); tileset++) {
							if ((*tileset)->firstgid > (*iter)->tiles[tile])
								continue;

							iPoint tile_pos = MapToWorld(x, y);
							SDL_Rect tile_rect = (*tileset)->GetTileRect((*iter)->tiles[tile]);
							App->render->Blit((*tileset)->texture, tile_pos.x, tile_pos.y, &tile_rect, (*iter)->parallax_speed);
					}
					tile++;
				}
			}
		}
	}
}

iPoint ModuleMap::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint ModuleMap::MapToWorld(const iPoint p) const
{
	iPoint ret;

	ret.x = p.x * data.tile_width;
	ret.y = p.y * data.tile_height;

	return ret;
}

iPoint ModuleMap::WorldToMap(int x, int y) const
{
	iPoint ret;

	ret.x = x / data.tile_width;
	ret.y = y / data.tile_height;

	return ret;
}

iPoint ModuleMap::WorldToMap(const iPoint p) const
{
	iPoint ret;

	ret.x = p.x / data.tile_width;
	ret.y = p.y / data.tile_height;

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool ModuleMap::CleanUp(pugi::xml_node&)
{
	LOG("Unloading map");

	// Remove all tilesets
	for (std::list<TileSet*>::reverse_iterator tileset = data.tilesets.rbegin(); tileset != data.tilesets.rend(); tileset++) {
		Utils::Release(*tileset);
	}
	data.tilesets.clear();

	
	// Remove all layers
	for (std::list<MapLayer*>::reverse_iterator layer = data.layers.rbegin(); layer != data.layers.rend(); layer++) {
		Utils::Release(*layer);
	}
	data.layers.clear();

	// Clean up the pugi tree
	map_file.reset();
	
	return true;
}

// Load new map
bool ModuleMap::Load(const char* fileName)
{
	bool ret = true;
	std::string tmp(folder);
	tmp.append(fileName);
	
	pugi::xml_parse_result result = map_file.load_file(tmp.c_str());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", fileName, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, *set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, *set);
		}

		data.tilesets.push_back(set);
	}

	
	// Load layer info ----------------------------------------------
	pugi::xml_node node_layer;
	uint i = 0;
	for (node_layer = map_file.child("map").child("layer"); node_layer && ret; node_layer = node_layer.next_sibling("layer"))
	{
		MapLayer* layer = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(node_layer, *layer);
			//App->pathfinding->SetMap(layer); //Load pathfinding map
			//App->pathfinding->SetGroundMap(layer);
		}

		data.layers.push_back(layer);
	}

	pugi::xml_node node_layer_objects;
	for (node_layer_objects = map_file.child("map").child("objectgroup"); node_layer_objects && ret; node_layer_objects = node_layer_objects.next_sibling("objectgroup"))
	{
		/*for (pugi::xml_node object : node_layer_objects.children()) {
			if (!strcmp(object.attribute("name").as_string(), "Player")) {
				initial_player_pos.x = object.attribute("x").as_float();
				initial_player_pos.y = object.attribute("y").as_float();
			}
			else if (!strcmp(object.attribute("name").as_string(), "Final"))
			{
				final_pos.x = object.attribute("x").as_float();
				final_pos.y = object.attribute("y").as_float();
			}
			else if (!strcmp(object.attribute("name").as_string(),"Enemy"))
			{
				float x = object.attribute("x").as_float();
				float y = object.attribute("y").as_float();
				int type;
				int layer;
				for (pugi::xml_node object2 : object.child("properties"))
				{
					if (!strcmp(object2.attribute("name").as_string(), "Layer"))
						layer = object2.attribute("value").as_int();
					else if (!strcmp(object2.attribute("name").as_string(), "Type"))
						type = object2.attribute("value").as_int();
				}
		
				App->entities->Add_Enemy(static_cast<BaseEnemy::Type>(type), { x, y }, static_cast<LayerID>(layer));
			}
		}*/
	}

	//background = App->textures->Load("textures/grid.png");

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", fileName);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		for (std::list<TileSet*>::iterator tileset = data.tilesets.begin(); tileset != data.tilesets.end(); tileset++) {
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", (*tileset)->name.c_str(), (*tileset)->firstgid);
			LOG("tile width: %d tile height: %d", (*tileset)->tile_width, (*tileset)->tile_height);
			LOG("spacing: %d margin: %d", (*tileset)->spacing, (*tileset)->margin);
		}

		
		for (std::list<MapLayer*>::iterator layer = data.layers.begin(); layer != data.layers.end(); layer++) {
			LOG("Layer ----");
			LOG("name: %s", (*layer)->name.c_str());
			LOG("tile width: %d tile height: %d", (*layer)->width, (*layer)->height);
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool ModuleMap::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		std::string bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.length() > 0)
		{
			std::string red, green, blue;
			red = bg_color.substr(1, 2);
			green = bg_color.substr(3, 4);
			blue = bg_color.substr(5, 6);

			int v = 0;

			sscanf_s(red.c_str(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.c_str(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.c_str(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		std::string orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool ModuleMap::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet& set)
{
	bool ret = true;
	set.name = tileset_node.attribute("name").as_string();
	set.firstgid = tileset_node.attribute("firstgid").as_int();
	set.tileCount = tileset_node.attribute("tilecount").as_int();
	set.tile_width = tileset_node.attribute("tilewidth").as_int();
	set.tile_height = tileset_node.attribute("tileheight").as_int();
	set.margin = tileset_node.attribute("margin").as_int();
	set.spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set.offset_x = offset.attribute("x").as_int();
		set.offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set.offset_x = 0;
		set.offset_y = 0;
	}

	return ret;
}

bool ModuleMap::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet& set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");
	std::string source = "Maps/";
	source.append(image.attribute("source").as_string());

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set.texture = App->textures->Load(source.c_str());
		int w, h;
		SDL_QueryTexture(set.texture, NULL, NULL, &w, &h);
		set.tex_width = image.attribute("width").as_int();

		if(set.tex_width <= 0) {
			set.tex_width = w;
		}

		set.tex_height = image.attribute("height").as_int();

		if(set.tex_height <= 0) {
			set.tex_height = h;
		}

		set.num_tiles_width = set.tex_width / set.tile_width;
		set.num_tiles_height = set.tex_height / set.tile_height;
	}

	return ret;
}


bool ModuleMap::LoadLayer(pugi::xml_node& node, MapLayer& layer)
{
	bool ret = true;
	pugi::xml_node data_node = node.child("data");
	pugi::xml_node properties = node.child("properties");

	if (data_node.root() == NULL) {
		return false;
	}

	for (pugi::xml_node object : properties.children()) {
		if (!strcmp(object.attribute("name").as_string(), "Speed"))
		{
			layer.parallax_speed = object.attribute("value").as_float();
		}
		else if (!strcmp(object.attribute("name").as_string(), "Collider")) {

		}
	}

	layer.name = node.attribute("name").as_string();
	layer.width = node.attribute("width").as_uint(0);
	layer.height = node.attribute("height").as_uint(0);

	if (layer.width == 0 || layer.height == 0) {
		return false;
	}

	layer.size = layer.height * layer.width;
	layer.tiles = new uint[layer.size];
	
	memset(layer.tiles, 0, layer.size);
	
	std::string encoding(data_node.attribute("encoding").as_string());

	uint tile_index = 0;
	if (encoding == "csv")
	{
		//p2SString data_buffer(data.child_value());
		uint buffer_index = 0;
		uint figures = 0;
		const char* aux_buf = data_node.child_value();//data_buffer.GetString();
		while (aux_buf[buffer_index] != '\0') {
			if (aux_buf[buffer_index] != ',' && aux_buf[buffer_index] != '\n')
				figures++;
			else {
				char* buf = (char*)malloc((figures + 1) * sizeof(char));

				//p2SString buffer = "";
				//data_buffer.SubString(buffer_index - figures, buffer_index, buffer);
				strncpy_s(buf, figures + 1 , &aux_buf[buffer_index - figures], _TRUNCATE);

				std::string buffer(buf);
				uint tile_id = (uint)Utils::ParseInt(buffer);
				layer.tiles[tile_index] = tile_id;
				figures = 0;
				free(buf);

				/*if (tile_id != 0)
				{
					Collider* aux = new Collider;
					iRect rect;
					rect.w = data.tile_width;
					rect.h = data.tile_height;
					rect.x = ((rect.w) * (tile_index % data.width));
					rect.y = ((rect.h) * (tile_index / data.width));
					aux->rect = rect;
					layer->layer_colliders.add(aux);
				}*/
				tile_index++;
			}
			buffer_index++;
		}
	}
	else {
		for (pugi::xml_node tile : data_node.children()) {
			uint tile_id = tile.first_attribute().as_uint(0);
			layer.tiles[tile_index] = tile_id;
			/*if (tile_id != 0 && type != DECORATION_LAYER)
			{
				Collider* aux = new Collider;
				iRect rect;
				rect.w = data.tile_width;
				rect.h = data.tile_height;
				rect.x = ((rect.w) * (tile_index % data.width));
				rect.y = ((rect.h) * (tile_index / data.width));
				aux->rect = rect;
				layer->layer_colliders.add(aux);
			}*/
			tile_index++;
		}
	}

	return true;
}

TileSet::~TileSet() {
	App->textures->UnLoad(texture);
	texture = nullptr;
}