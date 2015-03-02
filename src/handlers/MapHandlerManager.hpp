/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2015 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
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
#ifndef MAP_HANDLER_MANAGER_HPP
#define MAP_HANDLER_MANAGER_HPP

#include <list>
#include <map>
#include <string>

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

  /** Handles saving a map to the filesystem.
    * First the filename is scanned for its extension and then if a handler was not passed
    * in it will find the main handler handling that file type.  If a handler was passed in
    * then it will use that one instead.  The handler will then save the file.
    * @param filename Filepath to save to.
    * @param map Map to save.
    * @param handler Name of handler to use to save the file.
    */
  void Save(const std::string& filename, Map& map, const std::string& handler = "");
  /** Handles loading a map from the filesystem.
    * @see save for a description of how its loaded.
    * @param filename Filepath to load from.
    * @param map Map to load.
    * @param handler Optional Name of handler to use to load the file.
    */
  void Load(const std::string& filename, Map& map, const std::string& handler = "");
  /** Registers a handler with MapHandlerManager.
    * If two handlers handle the same main extension then the one that is added first will be used.
    * @param handler Handler to add.
    * @note This class will own the pointer once added.
    */
  void Add(BaseMapHandler* handler);
  /** Unregisters a handler with MapHandlerManager.
    * @param handler Name of handler to remove.
    */
  void Remove(const std::string& handler);
  /** Finds handler by name.
    * @param name Name of the handler to find
    * @return The handler or NULL if not found.
    * @note The pointer returned is owned by this class
    */
  BaseMapHandler* FindHandlerByName(const std::string& name);
  /** Finds handler that handles the given extension.
    * @param extension The extension you want the handler for.
    * @return The handler or NULL if not found.
    * @note The pointer returned is owned by this class
    */
  BaseMapHandler* FindHandlerByExtension(const std::string& extension);
  /** Gets all of the handlers able to load files
    * @return The list of readable handlers.
    * @note The pointers are returned are owned by this class
    */
  std::list<BaseMapHandler*> GetReadableHandlers();
  /** Gets all of the handlers able to save files
    * @return The list of writable handlers.
    * @note The pointers are returned are owned by this class
    */
  std::list<BaseMapHandler*> GetWriteableHandlers();
  /** Gets all of the readable and writable handlers
    * @return The list of all handlers.
    * @note The pointers are returned are owned by this class
    */
  std::list<BaseMapHandler*> GetHandlers();

  private:
  std::map<std::string, std::unique_ptr<BaseMapHandler>> handlers;
  MapHandlerManager() {};                                  // Private constructor
  MapHandlerManager(const MapHandlerManager&);             // Prevent copy-construction
  MapHandlerManager& operator=(const MapHandlerManager&);  // Prevent assignment
};

inline MapHandlerManager& MapHandlerManager() { return MapHandlerManager::Instance(); }

#endif
