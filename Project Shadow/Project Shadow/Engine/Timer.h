#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"

class Timer
{
public:

	Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;
	void SetZero();
	bool IsZero()const;
	void SubstractTime(Timer timer_to_substract_from);
	void AddTime(Timer timer_to_add_from);
	bool Count(float num);

private:
	uint32	started_at=0;
};

#endif //__TIMER_H__