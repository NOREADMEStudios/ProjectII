#include "Defs.h"
void Release(void* object) {
	delete object;
}

void ReleaseArray(void* array)
{
	delete[] array;
}
