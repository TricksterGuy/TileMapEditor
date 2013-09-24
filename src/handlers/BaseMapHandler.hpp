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

#ifndef BASEMAPHANDLER_HPP
#define BASEMAPHANDLER_HPP

#include <string>
#include <set>
#include "Map.hpp"

class BaseMapHandler
{
	public:
		BaseMapHandler(const std::string& name = "", const std::string& extension = "", const std::string& description = "");
		virtual ~BaseMapHandler();

        virtual int init();
        virtual int load(const std::string& filename, Map& map);
        virtual int save(const std::string& filename, Map& map);
        bool canRead() const;
        bool canWrite() const;

        const std::string& getExtension() const;
        const std::set<std::string> getAltExtensions() const;
        const std::string& getName() const;
        const std::string& getDescription() const;

        void setExtension(const std::string& extension);
        void setAltExtensions(std::set<std::string> alts);
        void setName(const std::string& name);
        void setDescription(const std::string& description);

	protected:
        std::string name;
        std::string extension;
        std::string description;
        bool readable;
        bool writeable;
        std::set<std::string> alternatives;
};

#endif
