#ifndef SPRITE_H
#define SPRITE_H

#include "setup.h"

const unsigned int texture_limit = 32;

struct texturePile
{
    sf::Texture list[texture_limit];
    const char *paths[texture_limit];

    unsigned int count = 0;
};

struct sprite
{
    float w, h, walkToX, walkToY;
    unsigned int spriteW, spriteH;
    unsigned int framesX, framesY;
    std::string path;
    sf::Sprite rect;
    unsigned int img;
    bool empty = true;

    sprite();

    sprite(const char *_path, float _x, float _y, unsigned int _fx, unsigned int _fy);

    void Put(float _x, float _y);
    void Move(float _xDist, float _yDist);
};

struct animation
{
    unsigned int start = 0, end = 0;

    float speed = 1.0f;
    float timer = 10.0f;
    unsigned int frame = 0;
    bool finished = false;
    float xScale = 1.0f, yScale = 1.0f;

    sprite *_sprite;

    animation()
    {
        _sprite = nullptr;
    }

    animation(sprite *sp, unsigned int s, unsigned int e = 1, float spd = 1.0f) : _sprite(sp)
    {
        start = s;
        end = e;
        speed = spd;
    }
    void setScale(float x, float y)
    {
        xScale = x;
        yScale = y;
    }

    void run(float delta_time, bool loop)
    {
        finished = false;
        if (timer <= 0.0f)
        {
            if (frame >= end)
                finished = true;

            timer = 10.0f;

            if (loop)
            {
                frame >= end ? frame = 0 : ++frame;
            }
            if (!loop)
            {
                frame >= end ? frame = end : ++frame;
            }
        }

        timer -= speed * delta_time;

        if (_sprite == nullptr)
        {
            std::cout << "error: animation not attached to a sprite!\n";
            finished = true;
            return;
        }
        _sprite->rect.setTextureRect(sf::IntRect(frame % _sprite->framesX * _sprite->spriteW, frame / _sprite->framesX * _sprite->spriteH,
                                                 _sprite->spriteW * xScale, _sprite->spriteH * yScale));
    }
};

void clearAllTextures();

#endif