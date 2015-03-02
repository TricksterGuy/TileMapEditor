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
#include "MapHandlerManager.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
//#include <boost/filesystem.hpp>

std::string GetExtension(const std::string& filename, std::string::size_type& idx)
{
  std::string extension;

  idx = filename.rfind('.');
  if (idx != std::string::npos)
    extension = filename.substr(idx + 1);

  std::transform(extension.begin(), extension.end(), extension.begin(), (int (*)(int))std::tolower);

  return extension;
}

std::string ConvertFilename(const std::string& filename)
{
  // boost::filesystem::path pathFrom = boost::filesystem::path(filename);
  // return pathFrom.string();
  return filename;
}

void MapHandlerManager::Add(BaseMapHandler* handler)
{
  handlers.emplace(handler->GetName(), std::unique_ptr<BaseMapHandler>(handler));
}

void MapHandlerManager::Remove(const std::string& name)
{
  handlers.erase(name);
}

void MapHandlerManager::Load(const std::string& file, Map& map, const std::string& name)
{
  std::string::size_type idx;
  std::string filename = ConvertFilename(file);
  std::string extension = GetExtension(filename, idx);
  std::transform(filename.begin() + idx, filename.end(), filename.begin() + idx, (int (*)(int))std::tolower);

  BaseMapHandler* handler = NULL;

  if (!name.empty())
    handler = FindHandlerByName(name);

  if (!handler)
    handler = FindHandlerByExtension(extension);

  if (!handler)
    throw "Handler not found for extension";

  //map.Destroy();
  handler->Load(filename, map);
}

void MapHandlerManager::Save(const std::string& file, Map& map, const std::string& name)
{
  std::string::size_type idx;
  std::string filename = ConvertFilename(file);
  std::string extension = GetExtension(filename, idx);
  transform(filename.begin() + idx, filename.end(), filename.begin() + idx, (int (*)(int))std::tolower);

  BaseMapHandler* handler = NULL;

  if (!name.empty())
    handler = FindHandlerByName(name);

  if (!handler)
    handler = FindHandlerByExtension(extension);

  if (!handler)
    throw "Handler not found for extension";

  handler->Save(filename, map);
}

BaseMapHandler* MapHandlerManager::FindHandlerByExtension(const std::string& extension)
{
  // Check all main extensions
  for (const auto& name_handler : handlers)
  {
    const auto& handler = name_handler.second;
    if (extension == handler->GetExtension())
      return handler.get();
  }

  // Check all alternative extensions for each handler
  for (const auto& name_handler : handlers)
  {
    const auto& handler = name_handler.second;
    const std::set<std::string> extensions = handler->GetAltExtensions();
    if (extensions.find(extension) != extensions.end())
      return handler.get();
  }

  return NULL;
}

BaseMapHandler* MapHandlerManager::FindHandlerByName(const std::string& name)
{
  if (handlers.find(name) == handlers.end())
    return NULL;

  return handlers.at(name).get();
}

std::list<BaseMapHandler*> MapHandlerManager::GetWriteableHandlers()
{
  std::list<BaseMapHandler*> writeable;

  for (const auto& name_handler : handlers)
  {
    const auto& handler = name_handler.second;
    if (handler->CanWrite())
      writeable.push_back(handler.get());
  }

  return writeable;
}

std::list<BaseMapHandler*> MapHandlerManager::GetReadableHandlers()
{
  std::list<BaseMapHandler*> readable;

  for (const auto& name_handler : handlers)
  {
    const auto& handler = name_handler.second;
    if (handler->CanRead())
      readable.push_back(handler.get());
  }

  return readable;
}

std::list<BaseMapHandler*> MapHandlerManager::GetHandlers()
{
  std::list<BaseMapHandler*> handler_list;

  for (const auto& name_handler : handlers)
  {
    const auto& handler = name_handler.second;
    if (handler->CanRead() || handler->CanWrite())
      handler_list.push_back(handler.get());
  }

  return handler_list;
}
