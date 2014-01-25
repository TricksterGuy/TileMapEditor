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

#include <algorithm>
#include <cctype>
#include <cstdio>
//#include <boost/filesystem.hpp>
#include "MapHandlerManager.hpp"

using namespace std;

 MapHandlerManager::~MapHandlerManager()
{
    list<BaseMapHandler*>::iterator it;

    for (it = handlers.begin(); it != handlers.end(); ++it)
    {
        delete *it;
    }
	handlers.clear();
}

void MapHandlerManager::Add(BaseMapHandler* handler)
{
    handlers.push_back(handler);
}

void MapHandlerManager::Remove(BaseMapHandler* handler)
{
    handlers.remove(handler);
}

void MapHandlerManager::Load(const std::string& file, Map& map, BaseMapHandler* handler)
{
    std::string::size_type idx;
    std::string filename = ConvertFilename(file);
    std::string extension = GetExtension(filename, idx);
    transform(filename.begin() + idx, filename.end(), filename.begin() + idx, (int(*)(int)) std::tolower);
    if (!handler)
        handler = FindHandler(extension);

    if (!handler)
        throw "Handler not found for extension";

    map.Destroy();
    handler->Load(filename, map);
}

void MapHandlerManager::Save(const std::string& file, Map& map, BaseMapHandler* handler)
{
    std::string::size_type idx;
    std::string filename = ConvertFilename(file);
    std::string extension = GetExtension(filename, idx);
    transform(filename.begin() + idx, filename.end(), filename.begin() + idx, (int(*)(int)) std::tolower);

    if (!handler)
        handler = FindHandler(extension);

    if (!handler)
        throw "Handler not found for extension";

    handler->Save(filename, map);
}

BaseMapHandler* MapHandlerManager::FindHandler(const std::string& extension)
{
    // Check all main extensions
    list<BaseMapHandler*>::iterator it;

    for (it = handlers.begin(); it != handlers.end(); ++it)
    {
        BaseMapHandler* handler = *it;
        if (extension == handler->GetExtension())
            return handler;
    }

    // Check all alternative extensions for each handler
    for (it = handlers.begin(); it != handlers.end(); ++it)
    {
        BaseMapHandler* handler = *it;
        const std::set<std::string> extensions = handler->GetAltExtensions();
        if (extensions.find(extension) != extensions.end())
            return handler;
    }

    return NULL;
}

std::list<BaseMapHandler*> MapHandlerManager::GetWriteableHandlers()
{
    std::list<BaseMapHandler*> writeable;
    list<BaseMapHandler*>::iterator it;

    for (it = handlers.begin(); it != handlers.end(); ++it)
    {
        BaseMapHandler* handler = *it;
        if (handler->CanWrite())
            writeable.push_back(handler);
    }

    return writeable;
}

std::list<BaseMapHandler*> MapHandlerManager::GetReadableHandlers()
{
    std::list<BaseMapHandler*> readable;
    list<BaseMapHandler*>::iterator it;

    for (it = handlers.begin(); it != handlers.end(); ++it)
    {
        BaseMapHandler* handler = *it;
        if (handler->CanRead())
            readable.push_back(handler);
    }

    return readable;
}

const std::string MapHandlerManager::GetExtension(const std::string& filename, std::string::size_type& idx)
{
    idx = filename.rfind('.');
    std::string extension;

    if (idx != std::string::npos)
        extension = filename.substr(idx + 1);
    else
        extension = "";

    transform(extension.begin(), extension.end(), extension.begin(), (int(*)(int)) std::tolower);

    return extension;
}

const string MapHandlerManager::ConvertFilename(const std::string& filename)
{
    //boost::filesystem::path pathFrom = boost::filesystem::path(filename);
    //return pathFrom.string();
    return filename;
}
