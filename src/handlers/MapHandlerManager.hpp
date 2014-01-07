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

/** This class is responsible for keeping track of all of the handlers loaded.
  * And is able to tell you which handler to use to save a file given its extension.
  */
class MapHandlerManager
{
	public:
        static MapHandlerManager& Instance()
        {
            static MapHandlerManager singleton;
            return singleton;
        }
		~MapHandlerManager();

        /** Handles saving a map to the filesystem.
          * First the filename is scanned for its extension and then if a handler was not passed
          * in it will find the main handler handling that file type.  If a handler was passed in
          * then it will use that one instead.  The handler will then save the file.
          * @param filename Filepath to save to.
          * @param map Map to save.
          * @param handler Optional handler to use to save the file.
          */
		void Save(const std::string& filename, Map& map, BaseMapHandler* handler = NULL);
		/** Handles loading a map from the filesystem.
		  * @see save for a description of how its loaded.
          * @param filename Filepath to load from.
          * @param map Map to load.
          * @param handler Optional handler to use to load the file.
          */
		void Load(const std::string& filename, Map& map, BaseMapHandler* handler = NULL);
		/** Registers a handler with MapHandlerManager.
          * If two handlers handle the same main extension then the one that is added first will be used.
          * @param handler Handler to add.
          */
		void Add(BaseMapHandler* handler);
		/** Unregisters a handler with MapHandlerManager.
          * @param handler Handler to remove.
          */
		void Remove(BaseMapHandler* handler);
		/** Finds handler that handles the given extension.
		  * @param extension The extension you want the handler for.
		  * @return The handler or NULL if not found.
		  */
		BaseMapHandler* FindHandler(const std::string& extension);
		/** Gets all of the handlers able to load files
		  * @return The list of readable handlers.
		  */
		std::list<BaseMapHandler*> GetReadableHandlers();
		/** Gets all of the handlers able to save files
		  * @return The list of writeable handlers.
		  */
		std::list<BaseMapHandler*> GetWriteableHandlers();
		/** Gets all of the handlers
		  * @return The list of all handlers.
		  */
		std::list<BaseMapHandler*>& GetHandlers() {return handlers;}

	private:
        std::list<BaseMapHandler*> handlers;
        MapHandlerManager() {};                                         // Private constructor
        MapHandlerManager(const MapHandlerManager&);                    // Prevent copy-construction
        MapHandlerManager& operator=(const MapHandlerManager&);         // Prevent assignment
        const std::string GetExtension(const std::string& filename, std::string::size_type& idx);
        const std::string ConvertFilename(const std::string& filename);
};

inline MapHandlerManager& MapHandlerManager()
{
    return MapHandlerManager::Instance();
}

#endif
