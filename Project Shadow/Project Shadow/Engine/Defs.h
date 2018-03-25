#ifndef __DEFS_H__
#define __DEFS_H__

#include <string>

#define ASSETS_ROOT "Assets/"
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

#endif