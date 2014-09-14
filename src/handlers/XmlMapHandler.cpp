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
#include "Logger.hpp"
#include "Scanner.hpp"

using namespace std;

class wxFInputStream : public wxInputStream
{
    public:
        wxFInputStream(std::istream& _stream) : stream(_stream) {}
    protected:
        size_t OnSysRead(void *buffer, size_t bufsize)
        {
            if (stream.bad())
            {
                m_lasterror = wxSTREAM_READ_ERROR;
                return 0;
            }
            else if (stream.eof())
            {
                m_lasterror = wxSTREAM_EOF;
                return 0;
            }

            stream.read(reinterpret_cast<char*>(buffer), bufsize);
            return stream.gcount();
        }
    private:
        std::istream& stream;
};

class wxFOutputStream : public wxOutputStream
{
    public:
        wxFOutputStream(std::ostream& _stream) : stream(_stream) {}
    protected:
        size_t OnSysWrite(const void *buffer, size_t bufsize)
        {
            VerboseLog("Writing %d bytes", bufsize);

            if (stream.bad())
            {
                m_lasterror = wxSTREAM_WRITE_ERROR;
                return 0;
            }

            stream.write(reinterpret_cast<const char*>(buffer), bufsize);
            return bufsize;
        }
    private:
        std::ostream& stream;
};

XmlMapHandler::XmlMapHandler() : BaseMapHandler("Xml Format", "xml", "Exports the map as an xml file")
{
}

XmlMapHandler::~XmlMapHandler()
{
}

void XmlMapHandler::Load(std::istream& file, Map& map)
{
    wxXmlDocument doc;
    wxFInputStream fis(file);
    if (!doc.Load(dynamic_cast<wxInputStream&>(fis)))
        throw "Could not open XML file";

    wxXmlNode* child = doc.GetRoot()->GetChildren();

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
        else if (name == "Animation")
            ReadAnimation(child, map);
        else
            throw "Unknown element found in file " + name;
    }
}

void XmlMapHandler::Save(std::ostream& file, const Map& map)
{
    wxXmlNode* root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, "Map");

    if (map.HasCollisionLayer())
        WriteCollision(root, map);
    for (int i = map.GetNumAnimatedTiles() - 1; i >= 0; i--)
        WriteAnimation(root, map, map.GetAnimatedTile(i));
    for (int i = map.GetNumBackgrounds() - 1; i >= 0; i--)
        WriteBackground(root, map, map.GetBackground(i));
    for (int i = map.GetNumLayers() - 1; i >= 0; i--)
        WriteLayer(root, map, map.GetLayer(i));
    WriteProperties(root, map);

    wxXmlDocument* doc = new wxXmlDocument();
    doc->SetRoot(root);

    wxOutputStream* fos = new wxFOutputStream(file);
    if (!doc->Save(*fos))
        throw "Failed to save XML file";
    delete fos;
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
        else if (property == "Tileset")
        {
            tileset = content;
        }
        else if (property == "TileDimensions")
        {
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
    DebugLog("Done Reading Properties");
}

void XmlMapHandler::ReadLayer(wxXmlNode* root, Map& map)
{
    DebugLog("Reading a Layer");

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
        else if (property == "Position")
        {
            int32_t x, y;
            if (!scanner.Next(x))
                throw "Could not parse position";
            if (!scanner.Next(y))
                throw "Could not parse position";
            attr.SetPosition(x, y);
        }
        else if (property == "Origin")
        {
            int32_t x, y;
            if (!scanner.Next(x))
                throw "Could not parse origin";
            if (!scanner.Next(y))
                throw "Could not parse origin";
            attr.SetOrigin(x, y);
        }
        else if (property == "Scale")
        {
            float x, y;
            if (!scanner.Next(x))
                throw "Could not parse scale";
            if (!scanner.Next(y))
                throw "Could not parse scale";
            attr.SetScale(x, y);
        }
        else if (property == "Rotation")
        {
            float rotation;
            if (!scanner.Next(rotation))
                throw "Could not parse rotation";
            attr.SetRotation(rotation);
        }
        else if (property == "Opacity")
        {
            float opacity;
            if (!scanner.Next(opacity))
                throw "Could not parse opacity";
            attr.SetOpacity(opacity);
        }
        else if (property == "BlendMode")
        {
            uint32_t mode;
            if (!scanner.Next(mode))
                throw "Could not parse blend mode";
            attr.SetBlendMode(mode);
        }
        else if (property == "BlendColor")
        {
            uint32_t color;
            if (!scanner.Next(color, 16))
                throw "Could not parse blend color";
            attr.SetBlendColor(color);
        }
        else if (property == "Priority")
        {
            int32_t priority;
            if (!scanner.Next(priority))
                throw "Could not parser priority";
            attr.SetDepth(priority);
        }
        else
        {
            throw "Unexpected token " + property;
        }

        child = child->GetNext();
    }

    if (data.size() != width * height)
        throw "Incorrect number of tile entries for layer";

    map.Add(Layer(name, width, height, data, attr));

    DebugLog("Done Reading Layer");
}

void XmlMapHandler::ReadBackground(wxXmlNode* root, Map& map)
{
    DebugLog("Reading a background");
    wxXmlNode* child = root->GetChildren();

    std::string name;
    std::string filename;
    int32_t mode;
    int32_t speedx = 0, speedy = 0;
    DrawAttributes attr;

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
        else if (property == "Filename")
        {
            filename = content;
        }
        else if (property == "Mode")
        {
            if (!scanner.Next(mode))
                throw "Could not parse mode";
        }
        else if (property == "Speed")
        {
            if (!scanner.Next(speedx))
                throw "Could not parse speed";
            if (!scanner.Next(speedy))
                throw "Could not parse speed";
        }
        else if (property == "Position")
        {
            int32_t x, y;
            if (!scanner.Next(x))
                throw "Could not parse position";
            if (!scanner.Next(y))
                throw "Could not parse position";
            attr.SetPosition(x, y);
        }
        else if (property == "Origin")
        {
            int32_t x, y;
            if (!scanner.Next(x))
                throw "Could not parse origin";
            if (!scanner.Next(y))
                throw "Could not parse origin";
            attr.SetOrigin(x, y);
        }
        else if (property == "Scale")
        {
            float x, y;
            if (!scanner.Next(x))
                throw "Could not parse scale";
            if (!scanner.Next(y))
                throw "Could not parse scale";
            attr.SetScale(x, y);
        }
        else if (property == "Rotation")
        {
            float rotation;
            if (!scanner.Next(rotation))
                throw "Could not parse rotation";
            attr.SetRotation(rotation);
        }
        else if (property == "Opacity")
        {
            float opacity;
            if (!scanner.Next(opacity))
                throw "Could not parse opacity";
            attr.SetOpacity(opacity);
        }
        else if (property == "BlendMode")
        {
            uint32_t mode;
            if (!scanner.Next(mode))
                throw "Could not parse blend mode";
            attr.SetBlendMode(mode);
        }
        else if (property == "BlendColor")
        {
            uint32_t color;
            if (!scanner.Next(color, 16))
                throw "Could not parse blend color";
            attr.SetBlendColor(color);
        }
        else if (property == "Priority")
        {
            int32_t priority;
            if (!scanner.Next(priority))
                throw "Could not parser priority";
            attr.SetDepth(priority);
        }
        else
        {
            throw "Unexpected token " + property;
        }

        child = child->GetNext();
    }

    map.Add(Background(name, filename, mode, speedx, speedy, attr));

    DebugLog("Done Reading a Background");
}

void XmlMapHandler::ReadAnimation(wxXmlNode* root, Map& map)
{
    DebugLog("Reading an animation");
    wxXmlNode* child = root->GetChildren();

    std::string name;
    int32_t delay;
    int32_t type;
    int32_t times;
    std::vector<int32_t> frames;

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
        else if (property == "Delay")
        {
            if (!scanner.Next(delay))
                throw "Could not parse name";
        }
        else if (property == "Type")
        {
            if (!scanner.Next(type))
                throw "Could not parse type";
        }
        else if (property == "Times")
        {
            if (!scanner.Next(times))
                throw "Could not parse times";
        }
        else if (property == "Frames")
        {
            while (scanner.HasMoreTokens())
            {
                int32_t element;
                if (!scanner.Next(element))
                    throw "Could not parse frames";
                frames.push_back(element);
            }
        }
        else
        {
            throw "Unexpected token " + property;
        }
        child = child->GetNext();
    }

    map.Add(AnimatedTile(name, delay, (Animation::Type)type, times, frames));
    DebugLog("Done Reading an Animation");
}

void XmlMapHandler::ReadCollision(wxXmlNode* root, Map& map)
{
    DebugLog("Reading Collision Layer");
    wxXmlNode* child = root->GetChildren();

    int32_t type = -1;
    uint32_t width = 0, height = 0;
    std::vector<int32_t> data;

    /// TODO this is correct for now when more types are implemented handle them
    while(child)
    {
        std::string property = child->GetName().ToStdString();
        std::string content = child->GetNodeContent().ToStdString();
        DebugLog("%s Got node %s content %s",  __func__, property.c_str(), content.c_str());

        Scanner scanner(content);
        if (property == "Type")
        {
            if (!scanner.Next(type))
                throw "Could not parse collision layer type";
        }
        else if (property == "Dimensions")
        {
            if (!scanner.Next(width))
                throw "Could not parse width";
            if (!scanner.Next(height))
                throw "Could not parse height";
        }
        else if (child->GetName() == "Data")
        {
            while (scanner.HasMoreTokens())
            {
                int32_t element;
                if (!scanner.Next(element))
                    throw "Could not parse data";
                data.push_back(element);
            }
        }
        else
        {
            throw "Unexpected token " + property;
        }

        child = child->GetNext();
    }

    if (data.size() != width * height)
        throw "Incorrect number of tile entries for collision layer";

    TileBasedCollisionLayer* clayer = new TileBasedCollisionLayer(width, height, data);
    map.SetCollisionLayer(clayer);

    DebugLog("Done Reading Collision Layer");
}

void XmlMapHandler::WriteProperties(wxXmlNode* root, const Map& map)
{
    DebugLog("Writing Properties");
    wxXmlNode* properties = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Properties");

    wxXmlNode* tiledimensions = new wxXmlNode(properties, wxXML_ELEMENT_NODE, "TileDimensions");
    new wxXmlNode(tiledimensions, wxXML_TEXT_NODE, "", wxString::Format("%i, %i", map.GetTileWidth(), map.GetTileHeight()));

    wxXmlNode* filename = new wxXmlNode(properties, wxXML_ELEMENT_NODE, "Tileset");
    new wxXmlNode(filename, wxXML_TEXT_NODE, "", map.GetFilename());

    wxXmlNode* name = new wxXmlNode(properties, wxXML_ELEMENT_NODE, "Name");
    new wxXmlNode(name, wxXML_TEXT_NODE, "", map.GetName());
    DebugLog("Done Writing Properties");
}

void XmlMapHandler::WriteLayer(wxXmlNode* root, const Map& map, const Layer& layer)
{
    DebugLog("Writing a Layer");
    wxXmlNode* node = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Layer");

    wxString layerdata = "\n";
    for (unsigned int i = 0; i < layer.GetHeight(); i++)
    {
        layerdata << "\t\t\t";
        for (unsigned int j = 0; j < layer.GetWidth(); j++)
            layerdata << layer.At(j, i) << ", ";
        layerdata << "\n";
    }

    wxXmlNode* data = new wxXmlNode(node, wxXML_ELEMENT_NODE, "Data");
    new wxXmlNode(data, wxXML_TEXT_NODE, "", layerdata);

    wxXmlNode* dimensions = new wxXmlNode(node, wxXML_ELEMENT_NODE, "Dimensions");
    new wxXmlNode(dimensions, wxXML_TEXT_NODE, "", wxString::Format("%i, %i", layer.GetWidth(), layer.GetHeight()));

    WriteAttributes(node, layer);

    wxXmlNode* name = new wxXmlNode(node, wxXML_ELEMENT_NODE, "Name");
    new wxXmlNode(name, wxXML_TEXT_NODE, "", layer.GetName());

    DebugLog("Done Writing a Layer");
}

void XmlMapHandler::WriteBackground(wxXmlNode* root, const Map& map, const Background& background)
{
    DebugLog("Writing a Background");
    int32_t x, y;
    background.GetSpeed(x, y);

    wxXmlNode* back = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Background");
    WriteAttributes(back, background);

    wxXmlNode* speed = new wxXmlNode(back, wxXML_ELEMENT_NODE, "Speed");
    new wxXmlNode(speed, wxXML_TEXT_NODE, "", wxString::Format("%i, %i", x, y));

    wxXmlNode* mode = new wxXmlNode(back, wxXML_ELEMENT_NODE, "Mode");
    new wxXmlNode(mode, wxXML_TEXT_NODE, "", wxString::Format("%i", background.GetMode()));

    wxXmlNode* filename = new wxXmlNode(back, wxXML_ELEMENT_NODE, "Filename");
    new wxXmlNode(filename, wxXML_TEXT_NODE, "", background.GetFilename());

    wxXmlNode* name = new wxXmlNode(back, wxXML_ELEMENT_NODE, "Name");
    new wxXmlNode(name, wxXML_TEXT_NODE, "", background.GetName());

    DebugLog("Done Writing a Background");
}

void XmlMapHandler::WriteAnimation(wxXmlNode* root, const Map& map, const AnimatedTile& animatedTile)
{
    DebugLog("Writing an Animation");
    wxXmlNode* animation = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Animation");

    wxString frameData = "\n\t\t\t";
    for (unsigned int i = 0; i < animatedTile.GetNumFrames(); i++)
    {
        frameData << animatedTile.GetFrame(i) << ", ";
    }
    frameData << "\n";

    wxXmlNode* frames = new wxXmlNode(animation, wxXML_ELEMENT_NODE, "Frames");
    new wxXmlNode(frames, wxXML_TEXT_NODE, "", frameData);

    wxXmlNode* times = new wxXmlNode(animation, wxXML_ELEMENT_NODE, "Times");
    new wxXmlNode(times, wxXML_TEXT_NODE, "", wxString::Format("%i", animatedTile.GetTimes()));

    wxXmlNode* type = new wxXmlNode(animation, wxXML_ELEMENT_NODE, "Type");
    new wxXmlNode(type, wxXML_TEXT_NODE, "", wxString::Format("%i", animatedTile.GetType()));

    wxXmlNode* delay = new wxXmlNode(animation, wxXML_ELEMENT_NODE, "Delay");
    new wxXmlNode(delay, wxXML_TEXT_NODE, "", wxString::Format("%i", animatedTile.GetDelay()));

    wxXmlNode* name = new wxXmlNode(animation, wxXML_ELEMENT_NODE, "Name");
    new wxXmlNode(name, wxXML_TEXT_NODE, "", animatedTile.GetName());

    DebugLog("Done Writing an Animation");
}

void XmlMapHandler::WriteCollision(wxXmlNode* root, const Map& map)
{
    DebugLog("Writing Collision Layer");
    TileBasedCollisionLayer* layer = dynamic_cast<TileBasedCollisionLayer*>(map.GetCollisionLayer());
    wxXmlNode* layern = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Collision");

    wxString layerdata = "\n";
    for (unsigned int i = 0; i < layer->GetHeight(); i++)
    {
        layerdata << "\t\t\t";
        for (unsigned int j = 0; j < layer->GetWidth(); j++)
            layerdata << layer->At(j, i) << ", ";
        layerdata << "\n";
    }

    wxXmlNode* data = new wxXmlNode(layern, wxXML_ELEMENT_NODE, "Data");
    new wxXmlNode(data, wxXML_TEXT_NODE, "", layerdata);

    wxXmlNode* dimensions = new wxXmlNode(layern, wxXML_ELEMENT_NODE, "Dimensions");
    new wxXmlNode(dimensions, wxXML_TEXT_NODE, "", wxString::Format("%i, %i", layer->GetWidth(), layer->GetHeight()));

    wxXmlNode* type = new wxXmlNode(layern, wxXML_ELEMENT_NODE, "Type");
    new wxXmlNode(type, wxXML_TEXT_NODE, "", wxString::Format("%i", layer->GetType()));

    DebugLog("Done Writing Collision Layer");
}

void XmlMapHandler::WriteAttributes(wxXmlNode* root, const DrawAttributes& attr)
{
    wxXmlNode* priority = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Priority");
    new wxXmlNode(priority, wxXML_TEXT_NODE, "", wxString::Format("%i", attr.GetDepth()));

    wxXmlNode* blendcolor = new wxXmlNode(root, wxXML_ELEMENT_NODE, "BlendColor");
    new wxXmlNode(blendcolor, wxXML_TEXT_NODE, "", wxString::Format("%X", attr.GetBlendColor()));

    wxXmlNode* blendmode = new wxXmlNode(root, wxXML_ELEMENT_NODE, "BlendMode");
    new wxXmlNode(blendmode, wxXML_TEXT_NODE, "", wxString::Format("%i", attr.GetBlendMode()));

    wxXmlNode* opacity = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Opacity");
    new wxXmlNode(opacity, wxXML_TEXT_NODE, "", wxString::Format("%f", attr.GetOpacity()));

    wxXmlNode* rotation = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Rotation");
    new wxXmlNode(rotation, wxXML_TEXT_NODE, "", wxString::Format("%f", attr.GetRotation()));

    wxXmlNode* scale = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Scale");
    new wxXmlNode(scale, wxXML_TEXT_NODE, "", wxString::Format("%f, %f", attr.GetScaleX(), attr.GetScaleY()));

    wxXmlNode* origin = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Origin");
    new wxXmlNode(origin, wxXML_TEXT_NODE, "", wxString::Format("%i, %i", attr.GetOriginX(), attr.GetOriginY()));

    wxXmlNode* position = new wxXmlNode(root, wxXML_ELEMENT_NODE, "Position");
    new wxXmlNode(position, wxXML_TEXT_NODE, "", wxString::Format("%i, %i", attr.GetX(), attr.GetY()));
}
