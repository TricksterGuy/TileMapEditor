/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2017 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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
#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <vector>
#include <wx/docview.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include "MapViewUpdate.hpp"

/**
  * This class is a clock that runs code to update views contents based on a framerate.
  * When creating an instance of this class be sure to call run after creating one.
  */
class Clock : public wxTimer {
public:
    Clock(unsigned int rate = 1);

    void Add(wxView* owner);
    void Remove(wxView* owner);
    void Pause();
    void Run();
    void Stop();
    bool IsPaused();

    unsigned int GetFramerate() const;
    void SetFramerate(unsigned int rate);

    virtual void Notify();

private:
    unsigned int framerate;
    bool paused;
    std::vector<wxView*> owners;
    MapViewUpdate update;
};

#endif
