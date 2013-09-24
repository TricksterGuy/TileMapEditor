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

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>

// Stupid Windows headers! you have to do this
#ifdef LoadImage
#undef LoadImage
#endif

//TODO Update this class to use reference counting!

class ResourceManager
{
    public:
        static ResourceManager& Instance()
        {
            static ResourceManager singleton;
            return singleton;
        }
        ~ResourceManager();
        sf::Texture& loadImage(const std::string& filename, bool& error);
        bool releaseImage(const std::string& filename);
        /*sf::Music& LoadMusic(const std::string& filename, bool& error);
        bool releaseMusic(const std::string& filename);
        sf::SoundBuffer& loadAudio(const std::string& filename, bool& error);
        bool releaseAudio(const std::string& filename);*/
    private:
        ResourceManager() {};                                       // Private constructor
        ResourceManager(const ResourceManager&);                    // Prevent copy-construction
        ResourceManager& operator=(const ResourceManager&);         // Prevent assignment
        std::map<std::string, sf::Texture*> images;
        //std::map<std::string, sf::SoundBuffer*> sounds;
        //std::map<std::string, sf::Music*> musics;
};

inline ResourceManager& ResourceManager()
{
    return ResourceManager::Instance();
}

#endif
