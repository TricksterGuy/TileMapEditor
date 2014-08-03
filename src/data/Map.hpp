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

#define MIN_TILE_SIZE ((uint32_t)4)
#define MAX_TILE_SIZE ((uint32_t)256)

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
		void Clear();
		/** Destroys the map releasing any resources it owns */
		void Destroy();
        /** Shifts the entire map in any direction a certain number of tiles.
          * @param horizontal amount to move left or right (negative = left).
          * @param vertical amount to move up or down (negative = up).
          * @param wrap if true any rows/columns will wrap around.
          */
		void Shift(int horizontal, int vertical, bool wrap = false);
        /** Resizes the map to fit the new dimensions.
          * @param newwidth the new width of the collision layer in tiles.
          * @param newheight the new height of the collision layer in tiles.
          * @param copy if true then don't destroy the layer in the process if false then clear out the layer.
          */
		void Resize(uint32_t width, uint32_t height, bool copy = true);

        void Add(const Layer& layer);
        void Add(const Background& back);
        void DeleteLayer(const uint32_t index);
        void DeleteBackground(const uint32_t index);

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
        uint32_t GetTileWidth() const {return tile_width;}
        uint32_t GetTileHeight() const {return tile_height;}
        const std::string& GetFilename() const {return filename;}
        const std::string& GetName() const {return name;}
        uint32_t GetNumLayers() const {return layers.size();}
        uint32_t GetNumBackgrounds() const {return backgrounds.size();}
        uint32_t GetNumAnimatedTiles() const {return animatedTiles.size();}

        std::vector<Layer>& GetLayers() {return layers;}
        std::vector<Background>& GetBackgrounds() {return backgrounds;}
        std::vector<AnimatedTile>& GetAnimatedTiles() {return animatedTiles;}
        Layer& GetLayer(const uint32_t index) {return layers[index];}
        Background& GetBackground(const uint32_t index) {return backgrounds[index];}
        AnimatedTile& GetAnimatedTile(const uint32_t index) {return animatedTiles[index];}

        CollisionLayer* GetCollisionLayer() const {return collisionLayer.get();}
        bool HasCollisionLayer() const {return collisionLayer != NULL;}

        void SetFilename(const std::string& _filename) {filename = _filename;}
        void SetName(const std::string& _name) {name = _name;}
        void SetLayers(const std::vector<Layer>& newLayers) {layers = newLayers;}
        void SetBackgrounds(const std::vector<Background>& newBackgrounds) {backgrounds = newBackgrounds;}
        void SetAnimatedTiles(const std::vector<AnimatedTile>& tiles) {animatedTiles = tiles;}
        void SetTileDimensions(uint32_t width, uint32_t height);
        void SetCollisionLayer(CollisionLayer* layer);

    private:
        std::string name;
        std::string filename;
        uint32_t tile_width, tile_height;
        std::vector<Layer> layers;
        std::vector<AnimatedTile> animatedTiles;
        std::vector<Background> backgrounds;
        std::unique_ptr<CollisionLayer> collisionLayer;
};
#endif
