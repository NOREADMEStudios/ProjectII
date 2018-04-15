#ifndef __POINT_H__
#define __POINT_H__
#include <math.h>
#include "../SDL/include/SDL_rect.h"

template<class TYPE>
class Point
{
public:

	TYPE x, y;

	Point()
	{
		x = 0, y = 0;
	}

	Point(const Point<TYPE>& v) {
		this->x = v.x;
		this->y = v.y;
	}

	Point(const TYPE& x, const TYPE& y) {
		this->x = x;
		this->y = y;
	}

	Point& create(const TYPE& x, const TYPE& y) {
		this->x = x;
		this->y = y;

		return(*this);
	}

	// Math ------------------------------------------------
	Point operator -(const Point &v) const {
		Point r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	Point operator + (const Point &v) const {
		Point r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	const Point& operator -=(const Point &v) {
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const Point& operator +=(const Point &v) {
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const Point& v) const {
		return (x == v.x && y == v.y);
	}

	bool operator !=(const Point& v) const {
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool IsZero() const {
		return (x == 0 && y == 0);
	}

	Point& SetToZero() {
		x = y = 0;
		return(*this);
	}

	Point& Negate() {
		x = -x;
		y = -y;

		return(*this);
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const Point& v) const {
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (TYPE)sqrtf((float)(fx*fx) + (fy*fy));
	}

	TYPE DistanceNoSqrt(const Point& v) const {
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx*fx) + (fy*fy);
	}

	TYPE DistanceManhattan(const Point& v) const {
		return abs(v.x - x) + abs(v.y - y);
	}

	Point operator *(const TYPE& f) const {
		Point<TYPE> p{
			x * f,
			y * f
		};
		return (p);
	}

	Point<int> to_iPoint() const {
		return Point<int>((int)x, (int)y);
	}

	Point<float> to_fPoint() const {
		return Point<float>((float)x, (float)y);
	}

	//Different from operator ==
	//Checks if the components are in a close range from eachother
	//Conceived for float points
	bool equals(const Point<float>& other) const
	{
		return (fabs(x - other.x) < 0.000001f && fabs(y - other.y) < 0.000001f);
	}

	bool close_to(const Point<float>& other, float radius) const
	{
		return (fabs(x - other.x) < radius && fabs(y - other.y) < radius);
	}

	float angle(const Point& other)
	{
		return atan2((float)(other.y - y), (float)(other.x - x));
	}

	float angle()
	{
		return atan2((float)(y), (float)(x));
	}
};

typedef Point<int> iPoint;
typedef Point<float> fPoint;
typedef Point<unsigned int> uiPoint;


#endif