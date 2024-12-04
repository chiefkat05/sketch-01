#ifndef DUNGEON_H
#define DUNGEON_H

#include <random>
#include <fstream>
#include "sprite.h"
#include "collision.h"

const unsigned int room_limit = 12;
const unsigned int width_limit = 128;
const unsigned int height_limit = 128;
const unsigned int collision_box_limit = 64;
const unsigned int entity_limit_2 = 64;

#define NULL_TILE -2147483647

struct tile
{
    int id = 0;
    bool collision, collisiontaken;
};

struct dungeon
{
    bool start = true, end = false, dungeonInitialized = false;
    tile tiles[width_limit][height_limit];
    sprite dungeonSprite;

    sprite enemies[entity_limit_2];
    int enemyCount = 0;

    unsigned int roomWidth = 0, roomHeight = 0;
    unsigned int tileSpriteX, tileSpriteY;
    float spawnLocationX = 0.0f, spawnLocationY = 0.0f;

    const char *tileSetPath;

    aabb collision_boxes[collision_box_limit];
    unsigned int collision_box_count = 0;

    float screenPositionX = 0.0f, screenPositionY = 0.0f, lastScreenPosX = 0.0f, lastScreenPosY = 0.0f, screenChangeDistanceX, screenChangeDistanceY;

    dungeon();
    dungeon(const char *_tileSetPath, const unsigned int xSize, const unsigned int ySize, float massScale, float massYOffset);

    void updateScreenPosition(float mouseX, float mouseY, float delta_time, float massScale, float massYOffset);

    void draw(sf::RenderWindow *win);

    void readRoomFile(const char *path, float mScale, float mYOffset);
};

#endif