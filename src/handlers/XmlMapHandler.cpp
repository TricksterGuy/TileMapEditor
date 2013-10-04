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

using namespace std;

XmlMapHandler::XmlMapHandler() : BaseMapHandler("Xml Format", "xml")
{
    description = "Exports the map as an xml file";
}

XmlMapHandler::~XmlMapHandler()
{

}

int XmlMapHandler::load(const std::string& filename, Map& map)
{
    wxXmlDocument doc;
    if (!doc.Load(filename.c_str())) return -1;

    wxXmlNode* child = doc.GetRoot()->GetChildren();

    // Ensure that the first Node in the XML file is the Properties Node
    if (child != NULL && child->GetName() != "Properties")
    {
        wxMessageBox("Properties must be the first node in the XML file", "Error");
        return -1;
    }

    while (child)
    {
        if (child->GetName() == "Properties")
        {
            if (readProperties(child, map)) return -1;
        }
        else if (child->GetName() == "Layer")
        {
            if (readLayer(child, map)) return -1;
        }
        else if (child->GetName() == "Background")
        {
            if (readBackground(child, map)) return -1;
        }
        else if (child->GetName() == "Collision")
        {
            if (readCollision(child, map)) return -1;
        }

        child = child->GetNext();
    }

    return 0;
}

int XmlMapHandler::save(const std::string& filename, Map& map)
{
    wxXmlNode* root = new wxXmlNode(wxXML_ELEMENT_NODE, "Map");

    if (map.hasCollisionLayer())
        writeCollision(root, map);
    for (unsigned int i = map.getNumBackgrounds(); i > 0; i--)
        writeBackground(root, map, i - 1);
    for (unsigned int i = map.getNumLayers(); i > 0; i--)
        writeLayer(root, map, i - 1);
    writeProperties(root, map);

    wxXmlDocument doc;
    doc.SetRoot(root);
    bool ret = doc.Save(filename, 4);

    return ret ? 0 : -1;
}

int XmlMapHandler::readProperties(wxXmlNode* root, Map& map)
{
    wxXmlNode* child = root->GetChildren();

    while(child)
    {
        if (child->GetName() == "Name")
            map.setName(child->GetNodeContent());
        else if (child->GetName() == "Filename")
            map.setFilename(child->GetNodeContent());
        else if (child->GetName() == "Dimensions")
        {
            int width = -1, height = -1;
            wxXmlNode* grandchild = child->GetChildren();
            while(grandchild)
            {
                if (grandchild->GetName() == "Width")
                    width = wxAtoi(grandchild->GetNodeContent());
                else if (grandchild->GetName() == "Height")
                    height = wxAtoi(grandchild->GetNodeContent());
                grandchild = grandchild->GetNext();
            }

            if (width <= 0 || height <= 0)
            {
                wxMessageBox("Width and Height are incorrect", "Error");
                return -1;
            }

            width = max(min(width, 1024), 1);
            height = max(min(height, 1024), 1);

            map.resize(width, height);
        }
        else if (child->GetName() == "TileDimensions")
        {
            int width = -1, height = -1;
            wxXmlNode* grandchild = child->GetChildren();
            while(grandchild)
            {
                if (grandchild->GetName() == "Width")
                    width = wxAtoi(grandchild->GetNodeContent());
                else if (grandchild->GetName() == "Height")
                    height = wxAtoi(grandchild->GetNodeContent());
                grandchild = grandchild->GetNext();
            }

            if (width <= 0 || height <= 0)
            {
                wxMessageBox("Width and Height are incorrect", "Error");
                return -1;
            }

            width = max(min(width, 128), 8);
            height = max(min(height, 128), 8);

            map.setTileDimensions(width, height);
        }
        child = child->GetNext();
    }

    return 0;
}

int XmlMapHandler::readLayer(wxXmlNode* root, Map& map)
{
    wxXmlNode* child = root->GetChildren();

    Layer layer("No Name", map.getWidth(), map.getHeight());

    while(child)
    {
        if (child->GetName() == "Name")
            layer.setName(child->GetNodeContent());
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
                if (i > layer.getWidth() * layer.getHeight()) break;
                layer.set(i, tileid);
                i++;
            }

            if (i != layer.getWidth() * layer.getHeight())
            {
                wxMessageBox("Not enough / Too Many tile entries for layer", "Error");
                return -1;
            }
        }

        child = child->GetNext();
    }

    map.add(layer);

    return 0;
}

int XmlMapHandler::readBackground(wxXmlNode* root, Map& map)
{
    wxXmlNode* child = root->GetChildren();

    Background background;

    float speedx = 0, speedy = 0;

    while(child)
    {
        if (child->GetName() == "Name")
            background.setName(child->GetNodeContent());
        else if (child->GetName() == "Filename")
            background.setFilename(child->GetNodeContent());
        else if (child->GetName() == "Mode")
            background.setMode(wxAtoi(child->GetNodeContent()));
        else if (child->GetName() == "SpeedX")
            speedx = wxAtof(child->GetNodeContent());
        else if (child->GetName() == "SpeedY")
            speedy = wxAtof(child->GetNodeContent());

        child = child->GetNext();
    }

    background.setSpeed(speedx, speedy);

    map.add(background);

    return 0;
}

int XmlMapHandler::readCollision(wxXmlNode* root, Map& map)
{
    wxXmlNode* child = root->GetChildren();

    Collision::Type type = (Collision::Type) -1;
    std::vector<int32_t> collision;
    collision.resize(map.getWidth() * map.getHeight());

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

            if (i != map.getWidth() * map.getHeight())
            {
                wxMessageBox("Not enough collision entries for collision layer", "Error");
                return -1;
            }
        }

        child = child->GetNext();
    }

    TileBasedCollisionLayer* clayer = new TileBasedCollisionLayer(map.getWidth(), map.getHeight(), collision);
    map.setCollisionLayer(clayer);

    return 0;
}

int XmlMapHandler::writeProperties(wxXmlNode* root, Map& map)
{
    wxXmlNode* properties = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Properties");

    wxXmlNode* filename = new wxXmlNode(properties, wxXML_ELEMENT_NODE, "Filename");
    new wxXmlNode(filename, wxXML_TEXT_NODE, "Filename", map.getFilename());

    wxXmlNode* name = new wxXmlNode(properties, wxXML_ELEMENT_NODE, "Name");
    new wxXmlNode(name, wxXML_TEXT_NODE, "Name", map.getName());

    wxXmlNode* tiledimensions = new wxXmlNode(properties, wxXML_ELEMENT_NODE, "TileDimensions");
    wxXmlNode* twidth = new wxXmlNode(tiledimensions, wxXML_ELEMENT_NODE, "Width");
    wxXmlNode* theight = new wxXmlNode(tiledimensions, wxXML_ELEMENT_NODE, "Height");
    new wxXmlNode(twidth, wxXML_TEXT_NODE, "Width", wxString::Format("%i", map.getTileWidth()));
    new wxXmlNode(theight, wxXML_TEXT_NODE, "Height", wxString::Format("%i", map.getTileHeight()));

    wxXmlNode* dimensions = new wxXmlNode(properties, wxXML_ELEMENT_NODE, "Dimensions");
    wxXmlNode* mwidth = new wxXmlNode(dimensions, wxXML_ELEMENT_NODE, "Width");
    wxXmlNode* mheight = new wxXmlNode(dimensions, wxXML_ELEMENT_NODE, "Height");
    new wxXmlNode(mwidth, wxXML_TEXT_NODE, "Width", wxString::Format("%i", map.getWidth()));
    new wxXmlNode(mheight, wxXML_TEXT_NODE, "Height", wxString::Format("%i", map.getHeight()));

    return 0;
}

int XmlMapHandler::writeLayer(wxXmlNode* root, Map& map, unsigned int i)
{
    Layer& layer = map.getLayer(i);

    wxXmlNode* layern = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Layer");

    wxString layerdata = "\n";
    for (unsigned int i = 0; i < layer.getHeight(); i++)
    {
        layerdata << "\t\t\t";
        for (unsigned int j = 0; j < layer.getWidth(); j++)
            layerdata << layer.at(j, i) << " ";
        layerdata << "\n";
    }

    wxXmlNode* data = new wxXmlNode(layern, wxXML_ELEMENT_NODE, "Data");
    new wxXmlNode(data, wxXML_TEXT_NODE, "Data", layerdata);

    wxXmlNode* name = new wxXmlNode(layern, wxXML_ELEMENT_NODE, "Name");
    new wxXmlNode(name, wxXML_TEXT_NODE, "Name", layer.getName());

    return 0;
}

int XmlMapHandler::writeBackground(wxXmlNode* root, Map& map, unsigned int i)
{
    Background& background = map.getBackground(i);
    float x, y;
    background.getSpeed(x, y);
    wxXmlNode* back = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Background");

    wxXmlNode* speedy = new wxXmlNode(back, wxXML_ELEMENT_NODE, "SpeedY");
    wxXmlNode* speedx = new wxXmlNode(back, wxXML_ELEMENT_NODE, "SpeedX");
    wxXmlNode* mode = new wxXmlNode(back, wxXML_ELEMENT_NODE, "Mode");
    wxXmlNode* filename = new wxXmlNode(back, wxXML_ELEMENT_NODE, "Filename");
    wxXmlNode* name = new wxXmlNode(back, wxXML_ELEMENT_NODE, "Name");

    new wxXmlNode(filename, wxXML_TEXT_NODE, "Filename", background.getFilename());
    new wxXmlNode(speedy, wxXML_TEXT_NODE, "SpeedY", wxString::Format("%f", y));
    new wxXmlNode(speedx, wxXML_TEXT_NODE, "SpeedX", wxString::Format("%f", x));
    new wxXmlNode(mode, wxXML_TEXT_NODE, "Mode", wxString::Format("%i", background.getMode()));
    new wxXmlNode(name, wxXML_TEXT_NODE, "Name", background.getName());

    return 0;
}

int XmlMapHandler::writeCollision(wxXmlNode* root, Map& map)
{
    TileBasedCollisionLayer* layer = dynamic_cast<TileBasedCollisionLayer*>(map.getCollisionLayer());
    wxXmlNode* layern = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Collision");

    wxString layerdata = "\n";
    for (unsigned int i = 0; i < layer->getHeight(); i++)
    {
        layerdata << "\t\t\t";
        for (unsigned int j = 0; j < layer->getWidth(); j++)
            layerdata << layer->at(j, i) << " ";
        layerdata << "\n";
    }

    wxXmlNode* data = new wxXmlNode(layern, wxXML_ELEMENT_NODE, "Data");
    new wxXmlNode(data, wxXML_TEXT_NODE, "Data", layerdata);

    wxXmlNode* mode = new wxXmlNode(layern, wxXML_ELEMENT_NODE, "Mode");
    new wxXmlNode(mode, wxXML_TEXT_NODE, "Mode", wxString::Format("%i", layer->getType()));

    return 0;
}
