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

#ifndef MAPHANDLERMANAGER_HPP
#define MAPHANDLERMANAGER_HPP

#include <string>
#include <list>
#include "BaseMapHandler.hpp"

class MapHandlerManager
{
	public:
        static MapHandlerManager& Instance()
        {
            static MapHandlerManager singleton;
            return singleton;
        }
		~MapHandlerManager();

		int save(const std::string& filename, Map& map, BaseMapHandler* handler = NULL);
		int load(const std::string& filename, Map& map, BaseMapHandler* handler = NULL);
		int add(BaseMapHandler* handler);
		int remove(BaseMapHandler* handler);
		BaseMapHandler* findHandler(const std::string& extension);
		std::list<BaseMapHandler*> getReadableHandlers();
		std::list<BaseMapHandler*> getWriteableHandlers();
		std::list<BaseMapHandler*> getHandlers() const;

	private:
        std::list<BaseMapHandler*> handlers;
        MapHandlerManager() {};                                         // Private constructor
        MapHandlerManager(const MapHandlerManager&);                    // Prevent copy-construction
        MapHandlerManager& operator=(const MapHandlerManager&);         // Prevent assignment
        const std::string getExtension(const std::string& filename, std::string::size_type& idx);
        const std::string convertFilename(const std::string& filename);
};

inline MapHandlerManager& MapHandlerManager()
{
    return MapHandlerManager::Instance();
}

#endif
