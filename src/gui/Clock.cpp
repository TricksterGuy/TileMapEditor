/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2013 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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

/** Clock
  *
  * Constructor
  * @param rate Framerate.
  */
Clock::Clock(unsigned int rate) : framerate(rate), paused(true), update()
{
    update.SetNeedRefresh(true);
}

/** ~Clock
  *
  * Destructor
  */
Clock::~Clock()
{

}

/** add
  *
  * Adds a window to the list of refreshed windows.
  * @param owner window to add
  */
void Clock::add(wxView* owner)
{
    owners.push_back(owner);
}

/** pause
  *
  * Pauses the clock.
  */
void Clock::pause()
{
    paused = true;
    Stop();
}

/** isPaused
  *
  * Tests if the clock is paused.
  * @return true if the window is paused false otherwise
  */
bool Clock::isPaused()
{
    return paused;
}

/** run
  *
  * Runs the clock.
  */
void Clock::run()
{
    paused = false;
    Start(1000 / framerate, wxTIMER_CONTINUOUS);
}

/** getFramerate
  */
unsigned int Clock::getFramerate() const
{
    return framerate;
}

/** setFramerate
  */
void Clock::setFramerate(unsigned int rate)
{
    framerate = rate;
}

/** Notify
  *
  * Called internally when all of the windows need to be refreshed.
  */
void Clock::Notify()
{
    for (unsigned int i = 0; i < owners.size(); i++)
    {
        owners[i]->OnUpdate(NULL, &update);
    }
}
