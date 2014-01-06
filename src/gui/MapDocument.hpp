#ifndef TME_DOCUMENT
#define TME_DOCUMENT

#include <wx/docview.h>
#include <wx/string.h>

#include "Map.hpp"

class MapDocument : public wxDocument
{
    public:
        virtual bool DeleteContents();
        virtual bool DoSaveDocument(const wxString& file);
        virtual bool DoOpenDocument(const wxString& file);
        Map& GetMap() {return map;}
    private:
        Map map;
        DECLARE_DYNAMIC_CLASS(MapDocument)
};

#endif
