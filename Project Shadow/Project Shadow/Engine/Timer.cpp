#include "Timer.h"
#include "..\SDL\include\SDL_timer.h"

Timer::Timer()
{
	Start();
}

void Timer::Start()
{
	started_at = SDL_GetTicks();
}

uint32 Timer::Read() const
{
	return SDL_GetTicks() - started_at;
}

float Timer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}

void Timer::SetZero()
{
	started_at = 0;
}

bool Timer::IsZero() const{

	return(started_at == 0);
}
void Timer::SubstractTime(Timer nwtime) {

	started_at += nwtime.Read();

}
void Timer::AddTime(Timer nwtime) {	

	started_at -= nwtime.Read();

}