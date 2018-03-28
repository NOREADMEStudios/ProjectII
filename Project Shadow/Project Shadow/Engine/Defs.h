#ifndef __DEFS_H__
#define __DEFS_H__

#include <string>

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
#define ENEMIES_DIR "Enemies/"
#define ENTITIES_DIR "Entities/"
#define INPUT_DIR "Input/"


typedef unsigned int uint32;
typedef unsigned int uint;
typedef unsigned long int uint64;

void Release(void* object);
void ReleaseArray(void* array);
int ParseInt(std::string);
int ParseInt(const char*);




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