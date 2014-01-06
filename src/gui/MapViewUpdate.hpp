#ifndef MAP_VIEW_UPDATE_HPP
#define MAP_VIEW_UPDATE_HPP

#include <wx/object.h>

class MapViewUpdate : public wxObject
{
    public:
        void SetUpdateMap(bool update) {updateMap = update;}
        bool GetUpdateMap() const {return updateMap;}
        void SetUpdateBackgrounds(bool update) {updateBackgrounds = update;}
        bool GetUpdateBackgrounds() const {return updateBackgrounds;}
        void SetUpdateTileset(bool update) {updateTileset = update;}
        bool GetUpdateTileset() const {return updateTileset;}
        void SetNeedRefresh(bool update) {needRefresh = update;}
        bool GetNeedRefresh() const {return needRefresh;}
    private:
        bool updateMap;
        bool updateBackgrounds;
        bool updateTileset;
        bool needRefresh;
        DECLARE_DYNAMIC_CLASS(MapViewUpdate)
};


#endif
