/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2014 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * excluding commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 *    you must not claim that you wrote the original software.
 *    An acknowledgement in your documentation and link to the original version is required.
 *
 * 2. Altered source versions must be plainly marked as such,
 *    and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 ******************************************************************************************************/

#include "Clock.hpp"

Clock::Clock(unsigned int rate) : framerate(rate), paused(true), update()
{
  update.SetNeedRefresh(true);
}

Clock::~Clock()
{
  owners.clear();
}

void Clock::Add(wxView* owner)
{
  owners.push_back(owner);
}

void Clock::Pause()
{
  paused = true;
  Stop();
}

bool Clock::IsPaused()
{
  return paused;
}

void Clock::Run()
{
  paused = false;
  Start(1000 / framerate, wxTIMER_CONTINUOUS);
}

void Clock::Stop()
{
  owners.clear();
  wxTimer::Stop();
}

unsigned int Clock::GetFramerate() const
{
  return framerate;
}

void Clock::SetFramerate(unsigned int rate)
{
  framerate = rate;
}

void Clock::Notify()
{
  for (unsigned int i = 0; i < owners.size(); i++)
  {
    owners[i]->OnUpdate(NULL, &update);
  }
}
