#ifndef __ANIMATION_H__
#define __ANIMATION_H__


#include "Point.h"
#include "Rect.h"
#include <vector>


struct AnimationFrame {
	iRect rect;
	iPoint pivot;
	iRect result_rect;

public:
	SDL_Rect GetRectSDL() {
		return rect.toSDL_Rect();
	}
};

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	std::vector<AnimationFrame> frames;

	std::string name;

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

	bool LoadAnimationsfromXML(std::string file_name) {
		pugi::xml_parse_result result = animationFile.load_file(file_name.data());
		if (result != NULL) {
			std::string lop = "loop";
			std::string spd = "speed";
			for (pugi::xml_node anim = animationFile.child("map").child("objectgroup"); anim; anim = anim.next_sibling("objectgroup")) {
				if (anim.attribute("name").as_string() == name) {
					for (pugi::xml_node propert = anim.child("properties").child("property"); propert; propert = propert.next_sibling("property")) {
						if (strcmp(propert.attribute("name").as_string(),lop.data())) {
							loop = propert.attribute("value").as_bool();
						}
						else if (strcmp(propert.attribute("name").as_string(), spd.data())) {
							speed = propert.attribute("value").as_float();
						}
					}
					for (pugi::xml_node object = anim.child("object"); object; object=object.next_sibling("object")) {
						iRect frame = { object.attribute("x").as_int(), object.attribute("y").as_int(), object.attribute("width").as_int(), object.attribute("height").as_int() };
						this->PushBack(frame.toSDL_Rect());
					}
					return true;
				}
			}
		}
		return false;
	}

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