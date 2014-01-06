#include "MapDocument.hpp"
#include "MapHandlerManager.hpp"
#include <wx/msgdlg.h>

IMPLEMENT_DYNAMIC_CLASS(MapDocument, wxDocument)

bool MapDocument::DeleteContents()
{
    map.clear();
    return true;
}

bool MapDocument::DoSaveDocument(const wxString& file)
{
    try
    {
        MapHandlerManager().save(file.ToStdString(), map);
    }
    catch (const char* str)
    {
        wxMessageBox(str, _("Error"));
        return false;
    }
    return true;
}

bool MapDocument::DoOpenDocument(const wxString& file)
{
    try
    {
        MapHandlerManager().load(file.ToStdString(), map);
    }
    catch (const char* str)
    {
        wxMessageBox(str, _("Error"));
        return false;
    }
    return true;
}
