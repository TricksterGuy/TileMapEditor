#include "MapDocument.hpp"
#include "MapHandlerManager.hpp"

IMPLEMENT_DYNAMIC_CLASS(MapDocument, wxDocument)

bool MapDocument::DeleteContents()
{
    map.clear();
    return true;
}

bool MapDocument::DoSaveDocument(const wxString& file)
{
    return MapHandlerManager().save(file.ToStdString(), map) == 0;
}

bool MapDocument::DoOpenDocument(const wxString& file)
{
    int ret = MapHandlerManager().load(file.ToStdString(), map);
    return ret == 0;
}
