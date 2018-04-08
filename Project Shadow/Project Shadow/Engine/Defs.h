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

#define DEFAULT_RESOLUTION_X 1600
#define DEFAULT_RESOLUTION_Y 900

typedef unsigned int uint32;
typedef unsigned int uint;
typedef unsigned long int uint64;
typedef std::string String;

struct Collider;

typedef void (*Callback)(void*);


#define CURSOR_WIDTH 2
#define TMP_STRING_SIZE 4096

#define VECTOR(type) std::vector<type>
#define ARRAY(type) VECTOR(type)

#define LIST(type) std::list<type>
#define LIST_ITERATOR(type) LIST(type)::iterator
#define LIST_REVERSE_ITERATOR(type) LIST(type)::reverse_iterator


#define REMOVE_FROM_LIST(ptr, list, type)						\
	LIST_ITERATOR(type) it;										\
	for (it = list.begin(); it != list.end(); it++) {			\
		if (*it == ptr) break;									\
	}															\
	if (it != list.end())										\
		list.remove(*it);


namespace Utils {

	void Release(void* object);
	void ReleaseArray(void* array);

	int ParseInt(std::string);
	int ParseInt(const char*);

};

// Interpolates between two values at a cerain rate (step)
template<class TYPE>
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

#endif


