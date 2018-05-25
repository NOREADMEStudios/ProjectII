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
	iRect attack = { 0,0,0,0 };
	iRect defense = { 0,0,0,0 };
	iRect parry = { 0,0,0,0 };
	
	
};

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	float realSpeed;
	std::vector<AnimationFrame> frames;
	std::vector<AnimColls> coll_frames;
	std::vector<iRect> coll_spell;

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame), frames(anim.frames)
	{
		//SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	bool LoadAnimationsfromXML(std::string animationName, std::string fileName) {
		pugi::xml_document animationFile;
		pugi::xml_parse_result result = animationFile.load_file(fileName.data());
		if (result != NULL) {
			std::string lop = "loop";
			std::string spd = "speed";
			int iterator=1;
			for (pugi::xml_node anim = animationFile.child("map").child("objectgroup"); anim; anim = anim.next_sibling("objectgroup")) {
				if (anim.attribute("name").as_string() == animationName) {
					for (pugi::xml_node propert = anim.child("properties").child("property"); propert; propert = propert.next_sibling("property")) {
						if (strcmp(propert.attribute("name").as_string(), lop.data()) == 0) {
							loop = propert.attribute("value").as_bool();
						}
						else if (strcmp(propert.attribute("name").as_string(), spd.data()) == 0) {
							speed = propert.attribute("value").as_float();
							realSpeed = speed;
						}
					}
					for (pugi::xml_node object = anim.child("object"); object; object = object.next_sibling("object")) {
						iRect frame = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
						iPoint pivot = LoadFramePivotFromXML(animationFile.child("map").child("objectgroup"), animationName, iterator);
						//pivot = { pivot.x - frame.x, pivot.y - frame.y };
						this->PushBack(frame.toSDL_Rect(), pivot);
					}
					bool ret = LoadFrameCollidersFromXML(animationFile.child("map").child("objectgroup"), animationName);
					return ret;
				}
			}
		}
		return false;
	}

	iRect GetFeetColliderFromFrame() {
		int currFrame = this->current_frame;
		iRect collider = coll_frames.at(currFrame).feet;
		if (collider.IsZero()) {
			return collider;
		}
		iRect frame = frames.at(currFrame).rect;
		return { (collider.x - frame.x), collider.y - frame.y, collider.w, collider.h };
	}

	iRect GetHitBoxColliderFromFrame() {
		int currFrame = this->current_frame;
		iRect collider = coll_frames.at(currFrame).hitbox;
		if (collider.IsZero()) {
			return collider;
		}
		iRect frame = frames.at(currFrame).rect;
		return { (collider.x - frame.x), collider.y - frame.y, collider.w, collider.h };
	}

	iRect GetAtkColliderFromFrame() {
		int currFrame = this->current_frame;
		iRect collider = coll_frames.at(currFrame).attack;
		if (collider.IsZero()) {
			return collider;
		}
		iRect frame = frames.at(currFrame).rect;
		return { (collider.x - frame.x), collider.y - frame.y, collider.w, collider.h };
	}
	iRect GetDefColliderFromFrame() {
		int currFrame = this->current_frame;
		iRect collider = coll_frames.at(currFrame).defense;
		if (collider.IsZero()) {
			return collider;
		}
		iRect frame = frames.at(currFrame).rect;
		return { (collider.x - frame.x), collider.y - frame.y, collider.w, collider.h };
	}
	iRect GetParryColliderFromFrame() {
		int currFrame = this->current_frame;
		iRect collider = coll_frames.at(currFrame).parry;
		if (collider.IsZero()) {
			return collider;
		}
		iRect frame = frames.at(currFrame).rect;
		return { (collider.x - frame.x), collider.y - frame.y, collider.w, collider.h };
	}

	iRect GetSpellColliderFromFrame() {
		int currFrame = this->current_frame;
		iRect collider = coll_spell.at(currFrame);
		if (collider.IsZero()) {
			return collider;
		}
		iRect frame = frames.at(currFrame).rect;
		return { (collider.x - frame.x), collider.y - frame.y, collider.w, collider.h };
	
	}

	void PushBack(const SDL_Rect& rect, const iPoint& pivot = { 0, 0 })
	{
		iRect new_rect = { rect.x, rect.y, rect.w, rect.h };
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
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}
		return frames[(int)current_frame];
	}

	AnimationFrame& GetPausedFrame() {
		int i = getFrameIndex();
		speed = 0;
		return frames[i];
	}

	void ResumeFrame() {
	
		speed = realSpeed;
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

	void Finish()
	{
		current_frame = last_frame;
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
private:
	bool LoadFrameCollidersFromXML(pugi::xml_node objectgroup, std::string name) {
		std::string coll = "_colliders";
		std::string frame = "Frame";
		std::string colltype = "collider_type";
		coll = name + coll;
		bool ret = false;
		for (; objectgroup; objectgroup = objectgroup.next_sibling("objectgroup")) {
			if (objectgroup.attribute("name").as_string() == coll) {//dentro de idle_colliders
				int i = 1;
				ret = true;
				iRect feetColl(0, 0, 0, 0);
				iRect HitBoxColl(0, 0, 0, 0);
				iRect AtkColl(0, 0, 0, 0);
				iRect DefColl(0, 0, 0, 0);
				iRect ParryColl(0, 0, 0, 0);
				for (pugi::xml_node object = objectgroup.child("object"); object; object = object.next_sibling("object")) {
					pugi::xml_node prop = object.child("properties").child("property");
					if (prop.attribute("name").as_string() == frame) {
						if (prop.attribute("value").as_int() == i) {

							prop = prop.next_sibling("property");
							if (prop.attribute("name").as_string() == colltype) {
								if (prop.attribute("value").as_int() == 5) {//collider spell
									iRect spellColl ;
									spellColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };									 
									coll_spell.push_back(spellColl);
									i++;
								}
								if (prop.attribute("value").as_int() == 0) {//collider def
									DefColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
								}
								else if (prop.attribute("value").as_int() == 4) {//collider parry
									ParryColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
								}
								else if (prop.attribute("value").as_int() == 1) {//collider atk
									AtkColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
								}
								else if (prop.attribute("value").as_int() == 2) {//collider hitbox
									HitBoxColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
								}
								else if (prop.attribute("value").as_int() == 3) {//collider feet
									feetColl = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
									this->coll_frames.push_back({ feetColl,HitBoxColl, AtkColl, DefColl, ParryColl  });
									feetColl.ToZero();
									HitBoxColl.ToZero();
									AtkColl.ToZero();
									DefColl.ToZero();
									ParryColl.ToZero();
									i++;
								}
							}
						}
					}
				}
				return ret;
			}
		}
		if (ret == false) {
			LOG("Cannot find colliders for the animation");
		}
		return ret;
	}

	iPoint LoadFramePivotFromXML(pugi::xml_node objectgroup, std::string name, int &iterator) {
		std::string pivot = "_pivot";
		std::string frame = "Frame";		
		pivot = name + pivot;
		bool ret = false;
		for (; objectgroup; objectgroup = objectgroup.next_sibling("objectgroup")) {
			if (objectgroup.attribute("name").as_string() == pivot) {//dentro de idle_pivot
				
				int x, y;
				for (pugi::xml_node object = objectgroup.child("object"); object; object = object.next_sibling("object")) {
					pugi::xml_node prop = object.child("properties").child("property");
					
					if (prop.attribute("name").as_string() == frame && prop.attribute("value").as_int() == iterator) {
						iterator+=1;
						x = object.attribute("x").as_int();
						y = object.attribute("y").as_int();

						return iPoint(x, y);
					}
				}
			}
		}
		return iPoint(0, 0);
	}
};

#endif