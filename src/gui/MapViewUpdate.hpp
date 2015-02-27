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

#ifndef MAP_VIEW_UPDATE_HPP
#define MAP_VIEW_UPDATE_HPP

#include <wx/object.h>

class MapViewUpdate : public wxObject
{
  public:
  MapViewUpdate();
  ~MapViewUpdate() {}
  void SetUpdateMap(bool update) { updateMap = update; }
  bool GetUpdateMap() const { return updateMap; }
  void SetUpdateBackgrounds(bool update) { updateBackgrounds = update; }
  bool GetUpdateBackgrounds() const { return updateBackgrounds; }
  void SetUpdateTileset(bool update) { updateTileset = update; }
  bool GetUpdateTileset() const { return updateTileset; }
  void SetNeedRefresh(bool update) { needRefresh = update; }
  bool GetNeedRefresh() const { return needRefresh; }

  private:
  bool updateMap;
  bool updateBackgrounds;
  bool updateTileset;
  bool needRefresh;
  DECLARE_DYNAMIC_CLASS(MapViewUpdate)
};


#endif
