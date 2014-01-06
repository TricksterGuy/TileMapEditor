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

#ifndef XMLMAPHANDLER_HPP
#define XMLMAPHANDLER_HPP

#include <wx/xml/xml.h>
#include "BaseMapHandler.hpp"

/** Saves the map as an xml file see documentation for format */
class XmlMapHandler : public BaseMapHandler
{
	public:
		XmlMapHandler();
		~XmlMapHandler();
		/** @see BaseMapHandler::load */
        virtual void load(const std::string& filename, Map& map);
        /** @see BaseMapHandler::save */
        virtual void save(const std::string& filename, Map& map);
	private:
        void readProperties(wxXmlNode* root, Map& map);
        void readLayer(wxXmlNode* root, Map& map);
        void readBackground(wxXmlNode* root, Map& map);
        void readCollision(wxXmlNode* root, Map& map);
        void writeProperties(wxXmlNode* root, Map& map);
        void writeLayer(wxXmlNode* root, Map& map, unsigned int i);
        void writeBackground(wxXmlNode* root, Map& map, unsigned int i);
        void writeCollision(wxXmlNode* root, Map& map);
};

#endif
