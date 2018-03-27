#ifndef __DEFS_H__
#define __DEFS_H__

#include <string>
#include <list>

#define ASSETS_ROOT "Assets/"
#define AUDIO_FX_FOLDER "FX/"
#define AUDIO_BGM_FOLDER "BGM/"

typedef unsigned int uint32;
typedef unsigned int uint;
typedef unsigned long int uint64;

typedef void (*Callback)(void* ptr);


#define CURSOR_WIDTH 2
#define TMP_STRING_SIZE 4096

#define LIST(type) std::list<type>
#define LIST_ITERATOR(type) LIST(type)::iterator
#define LIST_REVERSE_ITERATOR(type) LIST(type)::reverse_iterator

#define MIN(x1, x2) (x1 > x2 ? x2 : x1)
#define MAX(x1, x2) (x1 < x2 ? x2 : x1)
#define CLAMP(val, min, max) (MIN(MAX(val, min), max))

#define REMOVE_FROM_LIST(ptr, list, type)						\
	LIST_ITERATOR(type) it;										\
	for (it = list.begin(); it != list.end(); it++) {			\
		if (*it == ptr) break;									\
	}															\
	if (it != list.end())										\
		list.remove(*it);


void Release(void* object);
void ReleaseArray(void* array);

int ParseInt(std::string);
int ParseInt(const char*);

#endif
