// === Map.cpp ===
#include "Map.h"

Map::Map() {}

Map::Map(std::string tileSheet, int row, int column) {
    if (!texture.loadFromFile(tileSheet)) {
        std::cerr << "Error loading tile sheet: " << tileSheet << std::endl;
    }
    else {
        std::cout << "✅ Map texture loaded: " << tileSheet << std::endl;
    }
    this->row = row;
    this->column = column;
}

void Map::draw(sf::RenderWindow& window, sf::Vector2f playerPos) {
    const int tileSize = 96;
    sf::Vector2f viewSize = window.getView().getSize();
    int tilesX = (int)(viewSize.x / tileSize) + 5;
    int tilesY = (int)(viewSize.y / tileSize) + 5;
    int startTileX = (int)(playerPos.x) / tileSize - tilesX / 2;
    int startTileY = (int)(playerPos.y) / tileSize - tilesY / 2;

    for (int x = 0; x < tilesX; ++x) {
        for (int y = 0; y < tilesY; ++y) {
            sf::Sprite tile;
            tile.setTexture(texture);
            tile.setTextureRect(sf::IntRect(column * 16, row * 16, 16, 16));
            float posX = (startTileX + x) * tileSize;
            float posY = (startTileY + y) * tileSize;
            tile.setPosition(posX, posY);
            tile.setScale(6.f, 6.f);
            window.draw(tile);
        }
    }
}