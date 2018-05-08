#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEntityManager.h"
#include "ModuleMap.h"


#include "ModuleInput.h"
#include "../Brofiler/Brofiler.h"	

#define CAMERA_SPEED 3

ModuleRender::ModuleRender() : Module()
{
	name="renderer";
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;

	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;

	camera_speed = CAMERA_SPEED;
}

ModuleRender::~ModuleRender()
{}

bool ModuleRender::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true || App->render->vsync)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
		vsync = true;
	}
		renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

bool ModuleRender::Start()
{
	LOG("render start");
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

bool ModuleRender::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool ModuleRender::Update(float dt)
{
	PrintFromQueue(SpriteOrderer, dt);
	CheckCameraPos();
	return true;
}

bool ModuleRender::PostUpdate()
{
	BROFILER_CATEGORY("Render_PostUpdate", Profiler::Color::Red);
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

bool ModuleRender::CleanUp(pugi::xml_node&)
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

bool ModuleRender::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

bool ModuleRender::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void ModuleRender::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void ModuleRender::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void ModuleRender::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, bool h_flip, int pivot_x, int pivot_y) const
{
	bool ret = true;
	float scale = App->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(-camera.x * speed * scale) + x * scale;
	rect.y = (int)(-camera.y * speed * scale) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (h_flip)
		flip = SDL_FLIP_HORIZONTAL;


	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::BlitGui(SDL_Texture * texture, int x, int y, const SDL_Rect * section, bool use_camera, float speed, double angle, bool h_flip, SDL_Color color, int pivot_x, int pivot_y)
{
	bool ret = true;

	float scale = 1.f;//App->win->GetScale();
	SDL_Rect rect;
	if (use_camera)
	{
		rect.x = (int)(x * scale) - (int)(camera.x * speed);
		rect.y = (int)(y * scale) - (int)(camera.y * speed);
	}
	else
	{
		rect.x = (int)(x * scale);
		rect.y = (int)(y * scale);
	}

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w = (int)(rect.w * scale);
	rect.h = (int)(rect.h * scale);

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (h_flip)
		flip = SDL_FLIP_HORIZONTAL;

	if (color.r != 255 && color.g != 255 && color.b != 255)
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);

	if (color.a != 255)
		SDL_SetTextureAlphaMod(texture, color.a);

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	if (color.r != 255 || color.g != 255 || color.b != 255)
		SDL_SetTextureColorMod(texture, 255, 255, 255);

	if (color.a != 255)
		SDL_SetTextureAlphaMod(texture, 255);

	return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, float speed, bool filled, bool use_camera) const
{
	bool ret = true;
	float scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)((-camera.x * speed * scale) + (rect.x * scale));
		rec.y = (int)((-camera.y * speed * scale) + (rect.y * scale));
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	float scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 /** scale*/, y1 /** scale*/, x2 /** scale*/, y2 /** scale*/);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	float scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void ModuleRender::SetCameraInitialPos()
{
	camera.y = 0;
	camera.x = 0;
}

void ModuleRender::CheckCameraPos()
{
	if (App->entities->numofplayers > 0) {
		float min_x = 0;
		float max_x = 0;
		float min_y = 0;
		float max_y = 0;

		App->entities->CheckMidPos(min_x, max_x);
		App->entities->CheckMidPosY(min_y, max_y);
		int mapwidth = App->map->GetMapWidth();
		int mapheight = App->map->GetMapHeight();
		float scale = App->win->GetScale();
		float mid_pos = (((max_x - min_x) / 2) + min_x);
		float mid_pos_y = (((max_y - min_y) / 2) + min_y);

		float diference = MAX((max_x - min_x), (max_y - min_y));

		float min_scale = (float)camera.h / (mapheight - (App->map->GetYTiles()) + 1);
		float new_scale = MAX_SCALE - ((diference / (mapheight - camera.h / MAX_SCALE)) * (MAX_SCALE - min_scale));
		new_scale = CLAMP(new_scale, min_scale, MAX_SCALE);
		App->win->SetScale(new_scale);

		camera.x = mid_pos - camera.w / scale;
		if (camera.x < 0) camera.x = 0;
		else if (camera.x + camera.w / scale > mapwidth) camera.x = mapwidth - camera.w / scale;

		camera.y = mid_pos_y - camera.h / scale;
		if (camera.y < 0) camera.y = 0;
		else if (camera.y + camera.h / scale > mapheight) camera.y = mapheight - camera.h / scale;
	}
}

SDL_Point ModuleRender::ScreenToWorld(int x, int y) const
{
	SDL_Point ret;
	float scale = App->win->GetScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}

void ModuleRender::FillQueue(Entity* entity)
{
		SpriteOrderer.push(entity);
	
}

void ModuleRender::PrintFromQueue(std::priority_queue<Entity*, std::vector<Entity*>, OrderCrit>& Queue, float dt)
{
	while (Queue.empty() == false)
	{
		Entity* first = Queue.top();

		first->Draw(dt);		
		Queue.pop();

	}
}

SDL_Texture * ModuleRender::Snapshot()
{
	SDL_Surface* surf = SDL_CreateRGBSurface(0, (int)camera.w, (int)camera.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_ARGB8888, surf->pixels, surf->pitch);
	SDL_Texture* ret = SDL_CreateTextureFromSurface(renderer, surf);//App->tex->LoadSurface(surf);
	SDL_FreeSurface(surf);
	return ret;
}

