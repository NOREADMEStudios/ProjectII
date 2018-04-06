#ifndef __ANIMATION_H__
#define __ANIMATION_H__


#include "Point.h"
#include "Rect.h"
#include <vector>
#include "Log.h"


struct AnimationFrame {
	iRect rect;
	iPoint pivot;
	iRect result_rect;

public:
	SDL_Rect GetRectSDL() {
		return rect.toSDL_Rect();
	}
};
struct AnimColls {
	iRect feet;
	iRect hitbox;
	iRect attack = {0,0,0,0};
};

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	std::vector<AnimationFrame> frames;
	std::vector<AnimColls> coll_frames;
	

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;

	pugi::xml_document animationFile;

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame), frames(anim.frames)
	{
		//SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	bool LoadAnimationsfromXML(std::string animationName, std::string fileName) {
		pugi::xml_parse_result result = animationFile.load_file(fileName.data());
		if (result != NULL) {
			std::string lop = "loop";
			std::string spd = "speed";			
			for (pugi::xml_node anim = animationFile.child("map").child("objectgroup"); anim; anim = anim.next_sibling("objectgroup")) {
				if (anim.attribute("name").as_string() == animationName) {
					for (pugi::xml_node propert = anim.child("properties").child("property"); propert; propert = propert.next_sibling("property")) {
						if (strcmp(propert.attribute("name").as_string(), lop.data()) == 0) {
							loop = propert.attribute("value").as_bool();
						}
						else if (strcmp(propert.attribute("name").as_string(), spd.data()) == 0) {
							speed = propert.attribute("value").as_float();
						}
					}
					for (pugi::xml_node object = anim.child("object"); object; object=object.next_sibling("object")) {
						iRect frame = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
						this->PushBack(frame.toSDL_Rect());
					}
					bool ret = LoadFrameCollidersFromXML(anim, animationName);
					return ret;
				}				
			}
		}
		return false;
	}
private://
	bool LoadFrameCollidersFromXML(pugi::xml_node objectgroup, std::string name) {
		std::string coll = "_colliders";
		std::string frame = "Frame";
		std::string colltype = "collider_type";
		coll = name + coll;
		bool ret = false;
		for (; objectgroup; objectgroup = objectgroup.next_sibling("objectgroup")) {
			if (objectgroup.attribute("name").as_string() == coll) {//dentro de idle_colliders

				for (int i = 1; i <= this->frames.size(); i++) {

					ret = true;
					iRect feetColl(0, 0, 0, 0);
					iRect HitBoxColl(0, 0, 0, 0);
					iRect AtkColl(0, 0, 0, 0);



					for (pugi::xml_node object = objectgroup.child("object"); object; object = object.next_sibling("object")) {
						//itera entre objects de idle colliders


						pugi::xml_node prop = object.child("properties").child("property");
						if (prop.attribute("name").as_string() == frame) {
							if (prop.attribute("value").as_int() == i) {
							
								prop = prop.next_sibling("property");
								if (prop.attribute("name").as_string() == colltype) {
									
									if( prop.attribute("value").as_int() == 1) {//collider feet
										feetColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
									}
									else if (prop.attribute("value").as_int() == 2) {//collider hitbox
										HitBoxColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
									}
									else if (prop.attribute("value").as_int() == 3) {//collider atk
										AtkColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };

									}		
								}
							}			
							else if (prop.attribute("value").as_int() == i + 1) {
								this->coll_frames.push_back({ feetColl, HitBoxColl, AtkColl });
								feetColl.ToZero();
								HitBoxColl.ToZero();
								AtkColl.ToZero();
								continue;
							}
						}						
					}








				}

				
			


				/*
				for (pugi::xml_node object = objectgroup.child("object"); object; object = object.next_sibling("object")) {
					
					ret = true;
					iRect feetColl;
					iRect HitBoxColl;
					iRect AtkColl = {0,0,0,0};

					if (object.child("properties").child("property").attribute("value").as_int() == 1) {//collider feet
						feetColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
					}
					else if (object.child("properties").child("property").attribute("value").as_int() == 2) {//collider hitbox
						HitBoxColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
					}
					else if (object.child("properties").child("property").attribute("value").as_int() == 3) {//collider atk
						AtkColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
					}
					this->coll_frames.push_back({ feetColl, HitBoxColl, AtkColl });
					
				}*/

			}
			
		}
		if (ret == false) {
			LOG("Cannot find colliders for the animation");
		}
		return ret;
	}
public://

	void PushBack(const SDL_Rect& rect, const iPoint& pivot = { 0, 0 })
	{
		iRect new_rect = { rect.x, rect.y, rect.w, rect.h};
		frames.push_back({ new_rect, pivot, new_rect - pivot });
		last_frame++;
	}
	

	AnimationFrame& CurrentFrame()
	{
		return frames[(int)current_frame];
	}

	AnimationFrame& GetCurrentFrame(float delta_time)
	{
		current_frame = current_frame + speed * delta_time;
		if(current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
		loops = 0;
		if (speed == 0.0f)
			speed = 0.15f;
	}

	int getFrameIndex() const
	{
		return (int)current_frame;
	}

	

	/*Animation &operator =(const Animation &anim) 
	{
		loop = anim.loop;
		speed = anim.speed;
		loops = anim.loops;
		name = anim.name;
		current_frame = anim.current_frame;
		last_frame = anim.last_frame;

		for (uint i = 0; i < anim.frames.size(); i++)
			frames.push_back({ anim.frames[i].rect, anim.frames[i].pivot, anim.frames[i].result_rect });

		return *this;
	}*/
};

#endif