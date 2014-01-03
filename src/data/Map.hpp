#ifndef MAP_HPP
#define MAP_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <boost/scoped_ptr.hpp>
#include "Layer.hpp"
#include "CollisionLayer.hpp"
#include "Background.hpp"
#include "AnimatedTile.hpp"

/** The main datstructure of the program.
  * This class keeps track of the dimensions of the map itself and its tiles.
  * The data for the map is stored in Layers, Backgrounds, and a CollisionLayer.
  * @see Layer
  * @see Background
  * @see CollisionLayer
  * @see AnimatedTile
  */
class Map
{
    public:
        /** Creates a map with default dimensions
          * dimensions (1, 1)
          * tile dimensions (8, 8)
          */
        Map(const std::string& name = "", const std::string& filename = "", uint32_t width = 1, uint32_t height = 1,
            uint32_t tile_width = 8, uint32_t tile_height = 8);
        Map(const Map& map);
        ~Map();
        Map& operator=(const Map& map);

        /** Clears the map. */
		void clear();
		/** Destroys the map releasing any resources it owns */
		void destroy();
        /** Shifts the entire map in any direction a certain number of tiles.
          * @param horizontal amount to move left or right (negative = left).
          * @param vertical amount to move up or down (negative = up).
          * @param wrap if true any rows/columns will wrap around.
          */
		void shift(int horizontal, int vertical, bool wrap = false);
        /** Resizes the map to fit the new dimensions.
          * @param newwidth the new width of the collision layer in tiles.
          * @param newheight the new height of the collision layer in tiles.
          * @param copy if true then don't destroy the layer in the process if false then clear out the layer.
          */
		void resize(uint32_t width, uint32_t height, bool copy = true);

        void add(const Layer& layer);
        void add(const Background& back);
        void deleteLayer(const uint32_t index);
        void deleteBackground(const uint32_t index);

        uint32_t getWidth() const {return width;}
        uint32_t getHeight() const {return height;}
        uint32_t getTileWidth() const {return tile_width;}
        uint32_t getTileHeight() const {return tile_height;}
        const std::string& getFilename() const {return filename;}
        const std::string& getName() const {return name;}
        uint32_t getNumLayers() const {return layers.size();}
        uint32_t getNumBackgrounds() const {return backgrounds.size();}
        uint32_t getNumAnimatedTiles() const {return animatedTiles.size();}

        std::vector<Layer>& getLayers() {return layers;}
        std::vector<Background>& getBackgrounds() {return backgrounds;}
        std::vector<AnimatedTile>& getAnimatedTiles() {return animatedTiles;}
        Layer& getLayer(const uint32_t index) {return layers[index];}
        Background& getBackground(const uint32_t index) {return backgrounds[index];}
        AnimatedTile& getAnimatedTile(const uint32_t index) {return animatedTiles[index];}

        CollisionLayer* getCollisionLayer() const {return collisionLayer.get();}
        bool hasCollisionLayer() const {return collisionLayer != NULL;}

        void setFilename(const std::string& _filename) {filename = _filename;}
        void setName(const std::string& _name) {name = _name;}
        void setLayers(const std::vector<Layer>& newLayers) {layers = newLayers;}
        void setBackgrounds(const std::vector<Background>& newBackgrounds) {backgrounds = newBackgrounds;}
        void setAnimatedTiles(const std::vector<AnimatedTile>& tiles) {animatedTiles = tiles;}
        void setTileDimensions(uint32_t width, uint32_t height);
        void setCollisionLayer(CollisionLayer* layer);

    private:
        std::string name;
        std::string filename;
        uint32_t width, height;
        uint32_t tile_width, tile_height;
        std::vector<Layer> layers;
        std::vector<AnimatedTile> animatedTiles;
        std::vector<Background> backgrounds;
        boost::scoped_ptr<CollisionLayer> collisionLayer;
};
#endif
