#ifndef __CUBE_H__
#define __CUBE_H__

#include "Rect.h"

template<class TYPE>
class Cube {

public:
	TYPE x, y, z, w, h, d;

	Cube(){
		x = y = z = w = h = d = 0;
	}
	Cube(TYPE x, TYPE y, TYPE z, TYPE w, TYPE h, TYPE d) : x(x), y(y), z(z), w(w), h(h), d(d){}
	~Cube(){}

	bool notZeroDims() {
		return (w != 0 && h != 0 && d != 0);
	}

	bool Intersect(const Cube& other, Cube* result = nullptr, bool lowerDimension = false) const {
		if (result != nullptr) {
			result->x = MAX(x, other.x);
			result->w = MIN(x + w, other.x + other.w) - result->x;
			result->y = MAX(y, other.y);
			result->h = MIN(y + h, other.y + other.h) - result->y;
			result->z = MAX(z, other.z);
			result->d = MIN(z + d, other.z + other.d) - result->z;
		}
		return (lowerDimension || (notZeroDims() && other.notZeroDims())) &&						//None of the cubes has a dimension with 0 value,
			(((x <= other.x && x + w >= other.x) || (x >= other.x && x <= other.x + other.w)) &&	//since that would make the object lower dimension
			((y <= other.y && y + h >= other.y) || (y >= other.y && y <= other.y + other.h)) &&		//and therefore not really able to collide in theory.
			((z <= other.z && z + d >= other.z) || (z >= other.z && z <= other.z + other.d)));		//It is arguable that a 2D object can collide with a 3D one.
	}

	Rect<TYPE> GetRectXY() {
		return { x, y, w, h };
	}

	const Cube& operator = (const Rect<TYPE>& rect) {
		x = rect.x;
		y = rect.y;
		w = rect.w;
		h = rect.h;

		return *this;
	}

};

typedef Cube<int> iCube;
#endif