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
#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <wx/tokenzr.h>

/** Scanner class wrapping wxStringTokenizer
  * Automatically scans and converts next token to various types
  */
class Scanner : public wxStringTokenizer {
public:
    /** Constructor
      * @param str String to use as tokenizer
      * @param delims Delimiters for str
      */
    Scanner(const std::string& str, const std::string& delims = " ,\t\r\n") : wxStringTokenizer(str, delims, wxTOKEN_STRTOK) {}
    /** Constructor
      * @param str String to use as tokenizer
      * @param delims Delimiters for str
      */
    Scanner(const wxString& str, const wxString& delims = " ,\t\r\n") : wxStringTokenizer(str, delims, wxTOKEN_STRTOK) {}
    ~Scanner() {}
    /** Reads next integer from Scanner
      * @param var value to store integer in
      * @param base Next token from scanner is treated as an integer in this base
      * @return true if the value was read successfully
      */
    bool Next(int& var, int base = 10);
    /** Reads next unsigned integer from Scanner
      * @param var value to store integer in
      * @param base Next token from scanner is treated as an integer in this base
      * @return true if the value was read successfully
      */
    bool Next(unsigned int& var, int base = 10);
    /** Reads next float from Scanner
      * @param var value to store float in
      * @return true if the value was read successfully
      */
    bool Next(float& var);
    /** Reads next double from Scanner
      * @param var value to store double in
      * @return true if the value was read successfully
      */
    bool Next(double& var);
    /** Reads next string from Scanner
      * @param var value to store string in
      * @return true if the value was read successfully
      */
    bool Next(std::string& var);
    /** Reads next line from Scanner (do not use)
      * @param var value to store string in
      * @return true if the value was read successfully
      */
    bool NextLine(std::string& var);
};

#endif
