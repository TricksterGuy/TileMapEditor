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

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <wx/tokenzr.h>

class Scanner : public wxStringTokenizer
{
    public:
        Scanner(const std::string& str, const std::string& delims = " ") : wxStringTokenizer(str, delims) {}
        ~Scanner() {}
        bool Next(int32_t& var, int base = 10);
        bool Next(uint32_t& var, int base = 10);
        bool Next(float& var);
        bool Next(std::string& var);
        bool NextLine(std::string& var);
};

#endif
