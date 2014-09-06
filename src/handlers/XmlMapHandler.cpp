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

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <wx/xml/xml.h>
#include "XmlMapHandler.hpp"
#include "TileBasedCollisionLayer.hpp"
#include <wx/wfstream.h>
#include <wx/stdstream.h>
#include "Logger.hpp"
#include "Scanner.hpp"

using namespace std;

XmlMapHandler::XmlMapHandler() : BaseMapHandler("Xml Format", "xml", "Exports the map as an xml file")
{
}

XmlMapHandler::~XmlMapHandler()
{

}

void XmlMapHandler::Load(const std::string& mapfile, Map& map)
{
    DebugLog("Loading %s", mapfile.c_str());
    wxFileInputStream wxfile(mapfile);
    if (!wxfile.IsOk())
        throw "Could not open file";

    wxStdInputStream out(wxfile);
    Load(out, map);
}

void XmlMapHandler::Save(const std::string& mapfile, const Map& map)
{
    DebugLog("Saving %s", mapfile.c_str());
    wxFileOutputStream wxfile(mapfile);
    if (!wxfile.IsOk())
        throw "Could not open file";

    wxStdOutputStream out(wxfile);
    Save(out, map);
}

void XmlMapHandler::Load(std::istream& file, Map& map)
{
    wxXmlDocument doc;
    if (!doc.Load(dynamic_cast<wxInputStream&>(file)))
        throw "Could not open XML file";

    wxXmlNode* child = doc.GetRoot()->GetChildren();

    // Ensure that the first Node in the XML file is the Properties Node
    if (child != NULL && child->GetName() != "Properties")
        throw "Properties must be the first node in the XML file";

    ReadProperties(child, map);
    while ((child = child->GetNext()))
    {
        std::string name = child->GetName().ToStdString();
        DebugLog("%s Got node %s",  __func__, name.c_str());

        if (name == "Layer")
            ReadLayer(child, map);
        else if (name == "Background")
            ReadBackground(child, map);
        else if (name == "Collision")
            ReadCollision(child, map);
        else
            throw "Unknown tag found in file " + name;
    }
}

void XmlMapHandler::Save(std::ostream& file, const Map& map)
{
    wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, "Map");

    if (map.HasCollisionLayer())
        WriteCollision(root, map);
    for (unsigned int i = map.GetNumBackgrounds(); i > 0; i--)
        WriteBackground(root, map, i - 1);
    for (unsigned int i = map.GetNumLayers(); i > 0; i--)
        WriteLayer(root, map, i - 1);
    WriteProperties(root, map);

    wxXmlDocument doc;
    doc.SetRoot(root);

    if (!doc.Save(dynamic_cast<wxOutputStream&>(file)))
        throw "Failed to save XML file";
}

void XmlMapHandler::ReadProperties(wxXmlNode* root, Map& map)
{
    DebugLog("Reading Properties");

    std::string name;
    std::string tileset;
    uint32_t tile_width = 8, tile_height = 8;

    wxXmlNode* child = root->GetChildren();
    while(child)
    {
        std::string property = child->GetName().ToStdString();
        std::string content = child->GetNodeContent().ToStdString();
        DebugLog("%s Got node %s content %s",  __func__, property.c_str(), content.c_str());
        if (property == "Name")
        {
            name = content;
        }
        else if (property == "Filename")
        {
            tileset = content;
        }
        else if (property == "TileDimensions")
        {
            DebugLog("Reading TileDimensions");
            Scanner scanner(content);
            if (!scanner.Next(tile_width))
                throw "Could not parse tile_width";
            if (!scanner.Next(tile_height))
                throw "Could not parse tile_height";
        }
        child = child->GetNext();
    }

    map.SetName(name);
    map.SetFilename(tileset);
    map.SetTileDimensions(tile_width, tile_height);
}

void XmlMapHandler::ReadLayer(wxXmlNode* root, Map& map)
{
    DebugLog("Reading Layer");
    wxXmlNode* child = root->GetChildren();
    std::string name;
    uint32_t width;
    uint32_t height;
    DrawAttributes attr;
    std::vector<int32_t> data;

    while(child)
    {
        std::string property = child->GetName().ToStdString();
        std::string content = child->GetNodeContent().ToStdString();
        DebugLog("%s Got node %s content %s",  __func__, property.c_str(), content.c_str());

        Scanner scanner(content);
        if (property == "Name")
        {
            name = content;
        }
        else if (property == "Dimensions")
        {
            DebugLog("Reading Dimensions");
            if (!scanner.Next(width))
                throw "Could not parse width";
            if (!scanner.Next(height))
                throw "Could not parse height";
        }
        else if (property == "Data")
        {
            while (scanner.HasMoreTokens())
            {
                int32_t element;
                if (!scanner.Next(element))
                    throw "Could not parse data";
                data.push_back(element);
            }
        }

        child = child->GetNext();
    }

    if (data.size() != width * height)
        throw "Incorrect number of tile entries for layer";

    map.Add(Layer(name, width, height, data));
}

void XmlMapHandler::ReadBackground(wxXmlNode* root, Map& map)
{
    wxXmlNode* child = root->GetChildren();

    Background background;

    float speedx = 0, speedy = 0;

    while(child)
    {
        if (child->GetName() == "Name")
            background.SetName(child->GetNodeContent().ToStdString());
        else if (child->GetName() == "Filename")
            background.SetFilename(child->GetNodeContent().ToStdString());
        else if (child->GetName() == "Mode")
            background.SetMode(wxAtoi(child->GetNodeContent()));
        else if (child->GetName() == "SpeedX")
            speedx = wxAtof(child->GetNodeContent());
        else if (child->GetName() == "SpeedY")
            speedy = wxAtof(child->GetNodeContent());

        child = child->GetNext();
    }

    background.SetSpeed(speedx, speedy);
    map.Add(background);
}

void XmlMapHandler::ReadCollision(wxXmlNode* root, Map& map)
{
    wxXmlNode* child = root->GetChildren();

    Collision::Type type = (Collision::Type) -1;
    std::vector<int32_t> collision;
    collision.resize(map.GetWidth() * map.GetHeight());

    /// TODO this is correct for now when more types are implemented handle them
    while(child)
    {
        if (child->GetName() == "Type")
            type = (Collision::Type) wxAtoi(child->GetNodeContent());
        else if (child->GetName() == "Data")
        {
            wxString data = child->GetNodeContent();
            wxStringTokenizer scanner(data);
            unsigned int i = 0;
            int tileid;

            while (scanner.HasMoreTokens())
            {
                wxString token = scanner.GetNextToken();
                tileid = wxAtoi(token);
                collision[i] = tileid;
                i++;
            }

            if (i != map.GetWidth() * map.GetHeight())
                throw "Incorrect number of collision entries for collision layer";
        }

        child = child->GetNext();
    }

    TileBasedCollisionLayer* clayer = new TileBasedCollisionLayer(map.GetWidth(), map.GetHeight(), collision);
    map.SetCollisionLayer(clayer);
}

void XmlMapHandler::WriteProperties(wxXmlNode* root, const Map& map)
{
    wxXmlNode* properties = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Properties");

    wxXmlNode* filename = new wxXmlNode(properties, wxXML_ELEMENT_NODE, "Filename");
    new wxXmlNode(filename, wxXML_TEXT_NODE, "Filename", map.GetFilename());

    wxXmlNode* name = new wxXmlNode(properties, wxXML_ELEMENT_NODE, "Name");
    new wxXmlNode(name, wxXML_TEXT_NODE, "Name", map.GetName());

    wxXmlNode* tiledimensions = new wxXmlNode(properties, wxXML_ELEMENT_NODE, "TileDimensions");
    wxXmlNode* twidth = new wxXmlNode(tiledimensions, wxXML_ELEMENT_NODE, "Width");
    wxXmlNode* theight = new wxXmlNode(tiledimensions, wxXML_ELEMENT_NODE, "Height");
    new wxXmlNode(twidth, wxXML_TEXT_NODE, "Width", wxString::Format("%i", map.GetTileWidth()));
    new wxXmlNode(theight, wxXML_TEXT_NODE, "Height", wxString::Format("%i", map.GetTileHeight()));

    wxXmlNode* dimensions = new wxXmlNode(properties, wxXML_ELEMENT_NODE, "Dimensions");
    wxXmlNode* mwidth = new wxXmlNode(dimensions, wxXML_ELEMENT_NODE, "Width");
    wxXmlNode* mheight = new wxXmlNode(dimensions, wxXML_ELEMENT_NODE, "Height");
    new wxXmlNode(mwidth, wxXML_TEXT_NODE, "Width", wxString::Format("%i", map.GetWidth()));
    new wxXmlNode(mheight, wxXML_TEXT_NODE, "Height", wxString::Format("%i", map.GetHeight()));
}

void XmlMapHandler::WriteLayer(wxXmlNode* root, const Map& map, unsigned int i)
{
    const Layer& layer = map.GetLayer(i);

    wxXmlNode* layern = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Layer");

    wxString layerdata = "\n";
    for (unsigned int i = 0; i < layer.GetHeight(); i++)
    {
        layerdata << "\t\t\t";
        for (unsigned int j = 0; j < layer.GetWidth(); j++)
            layerdata << layer.At(j, i) << " ";
        layerdata << "\n";
    }

    wxXmlNode* data = new wxXmlNode(layern, wxXML_ELEMENT_NODE, "Data");
    new wxXmlNode(data, wxXML_TEXT_NODE, "Data", layerdata);

    wxXmlNode* name = new wxXmlNode(layern, wxXML_ELEMENT_NODE, "Name");
    new wxXmlNode(name, wxXML_TEXT_NODE, "Name", layer.GetName());
}

void XmlMapHandler::WriteBackground(wxXmlNode* root, const Map& map, unsigned int i)
{
    const Background& background = map.GetBackground(i);
    int32_t x, y;
    background.GetSpeed(x, y);
    wxXmlNode* back = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Background");

    wxXmlNode* speedy = new wxXmlNode(back, wxXML_ELEMENT_NODE, "SpeedY");
    wxXmlNode* speedx = new wxXmlNode(back, wxXML_ELEMENT_NODE, "SpeedX");
    wxXmlNode* mode = new wxXmlNode(back, wxXML_ELEMENT_NODE, "Mode");
    wxXmlNode* filename = new wxXmlNode(back, wxXML_ELEMENT_NODE, "Filename");
    wxXmlNode* name = new wxXmlNode(back, wxXML_ELEMENT_NODE, "Name");

    new wxXmlNode(filename, wxXML_TEXT_NODE, "Filename", background.GetFilename());
    new wxXmlNode(speedy, wxXML_TEXT_NODE, "SpeedY", wxString::Format("%i", y));
    new wxXmlNode(speedx, wxXML_TEXT_NODE, "SpeedX", wxString::Format("%i", x));
    new wxXmlNode(mode, wxXML_TEXT_NODE, "Mode", wxString::Format("%i", background.GetMode()));
    new wxXmlNode(name, wxXML_TEXT_NODE, "Name", background.GetName());
}

void XmlMapHandler::WriteCollision(wxXmlNode* root, const Map& map)
{
    TileBasedCollisionLayer* layer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
    wxXmlNode* layern = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Collision");

    wxString layerdata = "\n";
    for (unsigned int i = 0; i < layer->GetHeight(); i++)
    {
        layerdata << "\t\t\t";
        for (unsigned int j = 0; j < layer->GetWidth(); j++)
            layerdata << layer->At(j, i) << " ";
        layerdata << "\n";
    }

    wxXmlNode* data = new wxXmlNode(layern, wxXML_ELEMENT_NODE, "Data");
    new wxXmlNode(data, wxXML_TEXT_NODE, "Data", layerdata);

    wxXmlNode* mode = new wxXmlNode(layern, wxXML_ELEMENT_NODE, "Mode");
    new wxXmlNode(mode, wxXML_TEXT_NODE, "Mode", wxString::Format("%i", layer->GetType()));
}
