#ifndef __DEFS_H__
#define __DEFS_H__

#include <string>
#include <list>
#include <vector>

#define ASSETS_ROOT "Assets/"

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define CLAMP( value, min, max ) ( MAX( MIN( value, max ), min ) )

#define AUDIO_FX_DIR "FX/"
#define AUDIO_BGM_DIR "BGM/"
#define MAPS_DIR "Maps/"
#define SCENES_DIR "Scenes/"
#define AUDIO_DIR "Audio/"
#define ANIMATIONS_DIR "Animations/"
#define TEXTURES_DIR "Textures/"
#define CHARACTERS_DIR "Characters/"
#define UI_DIR "UI/"
#define ENEMIES_DIR "Enemies/"
#define ENTITIES_DIR "Entities/"
#define INPUT_DIR "Input/"

#define DEFAULT_RESOLUTION_X 1920
#define DEFAULT_RESOLUTION_Y 1080

typedef unsigned int uint32;
typedef unsigned int uint;
typedef unsigned long int uint64;
typedef std::string String;

struct Collider;

typedef void (*Callback)(size_t arg_size, ...);


#define CURSOR_WIDTH 2
#define TMP_STRING_SIZE 4096

#define VECTOR(type) std::vector<type>
#define ARRAY(type) VECTOR(type)
#define VECTOR_ITERATOR(type) VECTOR(type)::iterator
#define VECTOR_REVERSE_ITERATOR(type) VECTOR(type)::reverse_iterator

#define LIST(type) std::list<type>
#define LIST_ITERATOR(type) LIST(type)::iterator
#define LIST_REVERSE_ITERATOR(type) LIST(type)::reverse_iterator


namespace Utils {
	void Release(void* object);
	void ReleaseArray(void* array);

	int ParseInt(std::string);
	int ParseInt(const char*);

	template<typename TYPE>
	size_t ClearList(typename std::list<TYPE>& list) {
		typename std::list<TYPE>::iterator it;
		for (it = list.begin(); it != list.end(); pos++, it++) {
			delete *it;
		}
		list.clear();
	}

	template<typename TYPE>
	size_t ClearVector(typename std::vector<TYPE>& vector) {
		typename std::vector<TYPE>::iterator it;
		for (it = vector.begin(); it != vector.end(); pos++, it++) {
			delete *it;
		}
		vector.clear();
	}

	template<typename TYPE>
	size_t FindInList(TYPE ptr, typename std::list<TYPE>& list) {
		size_t pos = 0;
		typename std::list<TYPE>::iterator it;
		for (it = list.begin(); it != list.end(); pos++, it++) {
			if (*it == ptr) break;
		}

		if (it != list.end()) {
			return pos;
		}
		return -1;
	}

	template<typename TYPE>
	size_t FindInVector(TYPE ptr, typename std::vector<TYPE>& vector) {
		size_t pos = 0;
		for (pos = 0; pos < vector.size(); pos++) {
			if (vector[pos] == ptr) return pos;
		}
		return -1;
	}
	
	template<typename TYPE>
	bool RemoveFromList(TYPE ptr, typename std::list<TYPE>& list)
	{
		typename std::list<TYPE>::iterator it;
		for (it = list.begin(); it != list.end(); it++) {
				if (*it == ptr) break;
		}

		if (it != list.end()) {
			list.remove(*it);
			return true;
		}
		return false;
	}

	template<typename TYPE>
	bool RemoveFromVector(TYPE ptr, typename std::vector<TYPE>& vector)
	{
		typename std::vector<TYPE>::iterator it;
		for (it = vector.begin(); it != vector.end(); it++) {
			if (*it == ptr) break;
		}

		if (it != vector.end()){
			vector.erase(it);
			return true;
		}
		return false;
	}

	// Interpolates between two values at a cerain rate (step)
	template<typename TYPE>
	TYPE Interpolate(TYPE val, TYPE target, TYPE step)
	{
		if (val > target)
			if (val - target < step)
				val = target;
			else val = val - step;
		else if (val < target)
			if (target - val < step)
				val = target;
			else val = val + step;
		else val = target;

		return val;
	}

};

#endif
