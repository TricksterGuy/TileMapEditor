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

#ifndef BASEMAPHANDLER_HPP
#define BASEMAPHANDLER_HPP

#include <string>
#include <set>
#include <iostream>
#include "Map.hpp"

/** Base class for all map handlers
  * Handlers handle loading and/or saving maps to various formats.
  * Handlers have a name, a main extension for files it handles, and a description.
  * In addition to these a handler may handle other file types in alternatives.
  * To create your own handler you must inherit from this class and optionally override
  * init, load, and save.  In your constructor you must call this classes constructor setting
  * all the variables appropriately.  You then must register your handler with MapHandlerManager.
  */
class BaseMapHandler
{
  public:
  /** Creates a handler.
    * @param name Name of the handler.
    * @param extension Main extension this handler handles.
    * @param description Description of what this handler does.
    * @param readable Is this handler able to load files.
    * @param writeable Is this handler able to save files.
    * @param alternatives Extra extensions this handler handles.
    */
  BaseMapHandler(const std::string& name = "", const std::string& extension = "", const std::string& description = "",
                 bool readable = true, bool writeable = true,
                 const std::set<std::string>& alternatives = std::set<std::string>());
  virtual ~BaseMapHandler();

  /** Initializes the handler. Called by MapHandlerManager
    * If nonzero is returned the handler will not be able to be used.
    * @return 0 on success.
    */
  virtual int Init();
  /** Loads a map given a filename
    * @param filename Path to the file to load
    * @param map Map object to load the map to.
    */
  virtual void Load(const std::string& filename, Map& map);
  /** Loads a map given a stream object
    * @param file Stream object to load data from
    * @param map Map object to load the map to.
    */
  virtual void Load(std::istream& file, Map& map);
  /** Saves a map given a filename
    * @param filename Path to the file to save to.
    * @param map Map object to save.
    */
  virtual void Save(const std::string& filename, const Map& map);
  /** Saves a map given a stream object
    * @param file Stream object to save data to.
    * @param map Map object to save.
    */
  virtual void Save(std::ostream& file, const Map& map);

  const std::string& GetName() const { return name; }
  const std::string& GetExtension() const { return extension; }
  const std::string& GetDescription() const { return description; }
  bool CanRead() const { return readable; }
  bool CanWrite() const { return writeable; }
  const std::set<std::string>& GetAltExtensions() const { return alternatives; }

  void SetName(const std::string& _name) { name = _name; }
  void SetExtension(const std::string& _extension) { extension = _extension; }
  void SetDescription(const std::string& _description) { description = _description; }
  void SetAltExtensions(const std::set<std::string>& alts) { alternatives = alts; }

  protected:
  std::string name;
  std::string extension;
  std::string description;
  bool readable;
  bool writeable;
  std::set<std::string> alternatives;
};

#endif
