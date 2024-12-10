#include "../headers/dungeon.h"

const unsigned int dungeonSpriteXFrames = 4;
const unsigned int dungeonSpriteYFrames = 2;

dungeon::dungeon()
{
    dungeonInitialized = false;
}
dungeon::dungeon(const char *_tileSetPath, const unsigned int xSize, const unsigned int ySize, float massScale, float massYOffset)
{
    tileSetPath = _tileSetPath;
    dungeonSprite = sprite(tileSetPath, 0.0f, 0.0f, dungeonSpriteXFrames, dungeonSpriteYFrames);
    dungeonInitialized = true;
    for (unsigned int x = 0; x < width_limit; ++x)
    {
        for (unsigned int y = 0; y < height_limit; ++y)
        {
            // tiles[x][y] = sprite(tileSetPath, static_cast<float>(x), static_cast<float>(y), xSize, ySize, 1, 1);
            // tiles[x][y].Put(static_cast<float>(x) * tiles[x][y].spriteW, static_cast<float>(y) * tiles[x][y].spriteH);
            tiles[x][y].id = -1;
        }
    } // the pointer to the ui_element's visual object is incorrect inside the element's update function
}

void dungeon::updateScreenPosition(float mouseX, float mouseY, float delta_time, float massScale, float massYOffset)
{
    // if (mouseX < 10.0f / massScale)
    //     screenPositionX += (80.0f / massScale - (mouseX * 8.0f)) * delta_time * massScale;
    // if (mouseX > 246.0f / massScale)
    //     screenPositionX -= ((mouseX - 246.0f / massScale) * 8.0f) * delta_time * massScale;
    // if (mouseY < 10.0f / massScale)
    //     screenPositionY += (80.0f / massScale - (mouseY * 8.0f)) * delta_time * massScale;
    // if (mouseY > 118.0f / massScale + (massYOffset * 2.0f / massScale))
    //     screenPositionY -= ((mouseY - (118.0f / massScale + (massYOffset * 2.0f / massScale))) * 8.0f) * delta_time * massScale;

    float screenXPanLimit = 10.0f + (static_cast<float>(std::max(0, static_cast<int>(roomWidth) - 14)) * dungeonSprite.spriteW);
    if (screenPositionX < -screenXPanLimit)
    {
        screenPositionX = -screenXPanLimit;
    }
    if (screenPositionX > 0.0f)
    {
        screenPositionX = 0.0f;
    }
    float screenYPanLimit = 10.0f + (static_cast<float>(std::max(0, static_cast<int>(roomHeight) - 7)) * dungeonSprite.spriteH);
    if (screenPositionY < -screenYPanLimit)
    {
        screenPositionY = -screenYPanLimit;
    }
    if (screenPositionY > 10.0f)
    {
        screenPositionY = 10.0f;
    }
}

void dungeon::draw(sf::RenderWindow *win)
{
    for (unsigned int x = 0; x < width_limit; ++x)
    {
        for (unsigned int y = 0; y < height_limit; ++y)
        {
            if (tiles[x][y].id == -1)
                continue;

            dungeonSprite.Put(x * dungeonSprite.spriteW, y * dungeonSprite.spriteH); // change this to not be hardcoded pls
            dungeonSprite.rect.setTextureRect(sf::IntRect(static_cast<int>(tiles[x][y].id % dungeonSprite.framesX) + 1,
                                                          static_cast<int>(tiles[x][y].id / dungeonSprite.framesX), dungeonSprite.spriteW, dungeonSprite.spriteH));
            win->draw(dungeonSprite.rect);
        }
    }

    screenChangeDistanceX = lastScreenPosX - screenPositionX;
    screenChangeDistanceY = lastScreenPosY - screenPositionY;

    lastScreenPosX = screenPositionX;
    lastScreenPosY = screenPositionY;
}

void dungeon::readRoomFile(const char *path, float mScale, float mYOffset)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cout << "Failed to open " << path << "\n";
        return;
    }

    std::string line;
    roomHeight = 0;

    for (int i = 0; i < width_limit; ++i)
    {
        for (int j = 0; j < height_limit; ++j)
        {
            tiles[i][j].id = -1;
        }
    }

    while (std::getline(file, line))
    {
        if (line.size() > roomWidth)
            roomWidth = line.size();

        unsigned int widthLim = std::min(static_cast<unsigned int>(line.size()), width_limit);
        for (unsigned int i = 0; i < widthLim; ++i)
        {
            switch (line[i])
            {
            case '0':
                tiles[i][roomHeight].id = -1;
                tiles[i][roomHeight].collisionID = -1;
                break;
            case '1':
                tiles[i][roomHeight].id = 0;
                tiles[i][roomHeight].collisionID = 0;
                break;
            case 's':
                tiles[i][roomHeight].id = 3;

                spawnLocationX = i * dungeonSprite.spriteW;
                spawnLocationY = roomHeight * dungeonSprite.spriteH;
                tiles[i][roomHeight].collisionID = -1;
                break;
            case 'e':
                tiles[i][roomHeight].id = 3;

                tiles[i][roomHeight].collisionID = 2;
                break;
            case '2':
                tiles[i][roomHeight].id = 5;
                // tiles[i][roomHeight].collisionID = 1;
                tiles[i][roomHeight].collisionID = 1;
            default:
                tiles[i][roomHeight].id = -1;
                tiles[i][roomHeight].collisionID = -1;
                break;
            }
        }

        ++roomHeight;
    }

    file.close();
    if (file.is_open())
    {
        std::cout << "file didn't close!\n";
    }

    // it's not as good as it could be, but I love this thing
    int collisionstartx = -1, collisionstarty = -1, collisionendx = -1, collisionendy = -1;
    for (int c = 0; c < collision_id_limit; ++c)
    {
        for (int y = 0; y < roomHeight; ++y)
        {
            for (int x = 0; x < roomWidth; ++x)
            {
                // if (tiles[x][y].id == -1 || !tiles[x][y].collision || tiles[x][y].collisiontaken)
                //     continue;

                // if current spot is valid for collision and collision box not started yet, start collision box
                if (tiles[x][y].collisionID == c && !tiles[x][y].collisiontaken && collisionstartx == -1)
                {
                    collisionstartx = x;
                    collisionstarty = y;
                }

                // if current spot is valid for collision and collision x not ended yet or x position is within start and end x bounds (when y is different)
                if (tiles[x][y].collisionID == c && !tiles[x][y].collisiontaken && (collisionendx == -1 || x >= collisionstartx && x < collisionendx))
                {
                    // std::cout << collisionstartx << " <= " << x << " < " << collisionendx << " adding box to collision\n";
                    tiles[x][y].collisiontaken = true;
                }

                // if collision x started and collision x not ended and x hits the end or the next tile lacks collision or the next tile is part of a different box, set end x to next tile
                if (collisionstartx != -1 && collisionendx == -1 && (x + 1 > roomWidth || tiles[x + 1][y].collisionID != c || tiles[x + 1][y].collisiontaken))
                {
                    // std::cout << x << ", rw " << roomWidth << ", next collisionid " << tiles[x + 1][y].collisionID
                    //           << ", current c " << c << ", next taken " << tiles[x + 1][y].collisiontaken << " x end found\n";
                    collisionendx = x + 1;
                    // collisionendy = y + 1;
                }
                // if (collisionendy != -1 && collisionendx == -1 && (tiles[x][y].collisionID != c || tiles[x][y].collisiontaken))
                // {
                //     collisionendx = x;
                // }

                // std::cout << "sx = " << collisionstartx << ", ex = " << collisionendx << ", " << x << " | sy = " << collisionstarty << ", ey = " << collisionendy << ", " << y << " hmm\n";
                // if end x found and current y is not start y and current x is between box bounds and either next y has no collision or is part of another box, set end y to next y tile
                if (collisionendy == -1 && (collisionendx != -1 && x >= collisionstartx && x < collisionendx &&
                                                (!tiles[x][y + 1].collisionID == c || tiles[x][y + 1].collisiontaken) ||
                                            y + 1 == roomHeight))
                {
                    collisionendy = y + 1;
                }
                // if the current tile is not part of the collision group and an x end has been found but not a y end, and x is inside it's bounds, set y to the current block
                if (collisionendy == -1 && collisionendx != -1 && x >= collisionstartx && x < collisionendx && tiles[x][y].collisionID != c)
                {
                    collisionendy = y;
                }

                // if collision is not being finished in both directions, skip to next tile
                if (collisionendx == -1 || collisionendy == -1)
                    continue;

                collision_boxes[collision_box_count] = aabb(collisionstartx * dungeonSprite.spriteW, collisionstarty * dungeonSprite.spriteH,
                                                            collisionendx * dungeonSprite.spriteW, collisionendy * dungeonSprite.spriteH);
                collision_boxes[collision_box_count].collisionID = c;
                ++collision_box_count;
                if (collision_box_count >= collision_box_limit)
                {
                    std::cerr << "\n\tToo many collision boxes! Try increasing the collision box limit or lowering the number of boxes in your level.\n";
                    std::cerr << "\tfor information, there are currently " << collision_box_count << " collision boxes out of the limit of " << collision_box_limit << ".\n";
                }

                x = collisionstartx; // bad practice, make efficient
                y = collisionstarty; // no

                collisionstartx = -1;
                collisionstarty = -1;
                collisionendx = -1;
                collisionendy = -1;
            }
        }
    }
}