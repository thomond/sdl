#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../src/TileMapper.hpp"
#include "../src/Tilesheet.hpp"
#include "../src/Renderer.cpp"

TEST_CASE("TileMapper loads tilemap data from YAML") {
    auto tilemapData = TileMapper::loadTilemapData("../res/tilemap.yaml");
    
    for(auto& tile : tilemapData) {
        CAPTURE(tile.tile_id);
        for(auto& pos : tile.positions) {
            CAPTURE(pos.x, pos.y);
        }
    }
    REQUIRE(!tilemapData.empty());
}

TEST_CASE("TileMapper loads tilesheet data from YAML") {
    auto tilesheetData = Tilesheet::createFromYaml("../res/tilesheet.yaml");
    auto tile = tilesheetData.getTypeInfo(2); // Assuming tile ID 2 exists
    CAPTURE(tile->frame.x, tile->frame.y, tile->frame.w, tile->frame.h);
    REQUIRE(!tilesheetData.getTiles().empty());
}


TEST_CASE("Render Tilemap") {
    auto rend = Renderer::init();
    
    auto tilesheet = Tilesheet::createFromYaml("../res/tilesheet.yaml");
    auto tilemapData = TileMapper::loadTilemapData("../res/tilemap.yaml");
    auto layerDim = SDL_Point{800, 600}; // Example dimensions  
    auto layer = TileMapper::renderTilemap(layerDim, tilesheet, tilemapData);
    REQUIRE(layer != nullptr);
    REQUIRE(SDL_QueryTexture(layer, nullptr, nullptr, &layerDim.x, &layerDim.y) == 0);

    Renderer::addToRenderList(new GraphicsObject{layer, SDL_Rect{0, 0, layerDim.x, layerDim.y}});

    Renderer::draw(true);
    SDL_Event e;
    SDL_DestroyTexture(layer);
    Renderer::destroy();
}