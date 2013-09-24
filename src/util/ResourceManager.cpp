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

#include "ResourceManager.hpp"

/** ~ResourceManager
  *
  * Destructor
  */
ResourceManager::~ResourceManager()
{
	std::map<const std::string, sf::Texture*>::iterator i;
	//map<const string, SoundBuffer*>::iterator j;
	//map<const string, Music*>::iterator k;

	for(i = images.begin(); i != images.end(); ++i)
		delete i->second;
	//for(j = sounds.begin(); j != sounds.end(); ++j)
	//	delete j->second;
	//for(k = musics.begin(); k != musics.end(); ++k)
		//delete k->second;

	images.clear();
	//sounds.clear();
	//musics.clear();
}

/** LoadImage
  *
  * Loads an image into the Resource Manager
  */
sf::Texture& ResourceManager::loadImage(const std::string& filename, bool& error)
{
	std::map<const std::string, sf::Texture*>::iterator i;

	i = images.find(filename);
	if(i != images.end())
	{
	    error = false;
		return *i->second;
	}

    sf::Texture* image = new sf::Texture;
	error = !image->loadFromFile(filename);
	if (!error) images[filename] = image;

    return *image;
}

/** ReleaseImage
  *
  * Releases an image from the manager
  */
bool ResourceManager::releaseImage(const std::string& filename)
{
    std::map<const std::string, sf::Texture*>::iterator i;

    i = images.find(filename);
    if (i != images.end())
        return false;

    delete i->second;
    images.erase(i);
    return true;
}

///** LoadAudio
//  *
//  *
//  */
//SoundBuffer& ResourceManager::loadAudio(const string& filename, bool& error)
//{
//	map<const string, SoundBuffer*>::iterator i;
//	i = sounds.find(filename);
//	if(i != sounds.end())
//	{
//	    error = false;
//		return *i->second;
//	}
//
//    SoundBuffer* buffer = new SoundBuffer;
//	error = !buffer->loadFromFile(filename);
//	if (!error) sounds[filename] = buffer;
//
//    return *buffer;
//}
//
///** ReleaseAudio
//  *
//  *
//  */
//bool ResourceManager::releaseAudio(const string& filename)
//{
//    map<const string, SoundBuffer*>::iterator i;
//
//    i = sounds.find(filename);
//    if (i != sounds.end())
//        return false;
//
//    delete i->second;
//    sounds.erase(i);
//    return true;
//}
//
//
///** LoadMusic
//  *
//  *
//  */
//Music& ResourceManager::loadMusic(const string& filename, bool& error)
//{
//	map<const string, Music*>::iterator i;
//	i = musics.find(filename);
//	if(i != musics.end())
//	{
//	    error = false;
//		return *i->second;
//	}
//
//    Music* music = new Music;
//	error = !music->openFromFile(filename);
//	if (!error) musics[filename] = music;
//
//    return *music;
//}
//
///** ReleaseMusic
//  *
//  *
//  */
//bool ResourceManager::releaseMusic(const string& filename)
//{
//    map<const string, Music*>::iterator i;
//
//    i = musics.find(filename);
//    if (i != musics.end())
//        return false;
//
//    delete i->second;
//    musics.erase(i);
//    return true;
//}
