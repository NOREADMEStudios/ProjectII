#ifndef __DEFS_H__
#define __DEFS_H__

#include <string>

#define ASSETS_ROOT "Assets/"
#define AUDIO_FX_FOLDER "FX/"
#define AUDIO_BGM_FOLDER "BGM/"

typedef unsigned int uint32;
typedef unsigned int uint;
typedef unsigned long int uint64;

void Release(void* object);
void ReleaseArray(void* array);
int ParseInt(std::string);
int ParseInt(const char*);

#endif