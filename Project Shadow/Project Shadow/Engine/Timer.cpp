#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

j1Timer::j1Timer()
{
	Start();
}

void j1Timer::Start()
{
	started_at = SDL_GetTicks();
}

uint32 j1Timer::Read() const
{
	return SDL_GetTicks() - started_at;
}

float j1Timer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}

void j1Timer::SetZero()
{
	started_at = 0;
}

bool j1Timer::IsZero() const{

	return(started_at == 0);
}
void j1Timer::SubstractTime(j1Timer nwtime) {

	started_at += nwtime.Read();

}
void j1Timer::AddTime(j1Timer nwtime) {	

	started_at -= nwtime.Read();

}