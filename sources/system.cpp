#include "../headers/system.h"

soundhandler soundplayer;

// animation. When animation is on x frame, trigger ability. Ability consists of a list of targets, and a function. The function will take the targets and do something with them.
// void attack::clear()
// {
//     for (int i = 0; i < target_limit; ++i)
//     {
//         delete targets[i];
//         // targets[i] = nullptr;
//     }
// }
// void attack::updateTargets(character *tlist[target_limit], int tcount) // character is pre-defined in system pls fix :(
// {
//     clear();

//     for (int i = 0; i < tcount; ++i)
//     {
//         targets[i] = tlist[i];
//     }
//     targetCount = tcount;
// }

// ch_class::ch_class()
// {
//     maxHP = 10;
//     attackSpeed = 10.0f;
//     runSpeed = 100.0f;
//     attackRange = 100.0f;
//     experienceToLvlUp = 100;
// }
// ch_class::ch_class(std::string _n, int _mhp, int _attdmg, float _attspd, float _runspd, float _attrng, int _xpLvlUp, void ability_func(int, attack *, character *))
//     : name(_n), maxHP(_mhp), attackSpeed(_attspd),
//       runSpeed(_runspd), attackRange(_attrng),
//       experienceToLvlUp(_xpLvlUp)
// {
//     attackDamage = _attdmg;
//     for (int i = 0; i < att_limit; ++i)
//     {
//         abilities_list[i].distanceRequired = _attrng;
//         abilities_list[i].use = ability_func;
//     }
// }
// void ch_class::setAbility(int index, void _func(int, attack *, character *), float distanceMultiple = 1.0f, int _frame = 0, int _imageID = 0)
// {
//     abilities_list[index].use = _func;
//     abilities_list[index].distanceRequired = attackRange * distanceMultiple;
//     abilities_list[index].activationFrame = _frame;
//     abilities_list[index].imageID = _imageID;
// }

character::character() {}
character::character(sprite &v, IDENTIFICATION _id) : visual(v)
{
    visual.rect.setTextureRect(sf::IntRect(0, 0, visual.spriteW, visual.spriteH));
    visual.rect.setOrigin(sf::Vector2(static_cast<float>(visual.spriteW) * 0.5f, static_cast<float>(visual.spriteH)));
    // posX = visual.rect.getPosition().x;
    // posY = visual.rect.getPosition().y;
    // walkToX = posX;
    // walkToY = posY;
    id = _id;
    hp = maxhp;

    collider = aabb(visual.rect.getPosition().x - visual.spriteW * 0.5f, visual.rect.getPosition().y - visual.spriteH,
                    visual.rect.getPosition().x + visual.spriteW * 0.5f, visual.rect.getPosition().y);
}
character::character(std::string filepath, float x, float y, unsigned int fx, unsigned int fy, IDENTIFICATION _id)
{
    visual = sprite(filepath.c_str(), x, y, fx, fy);
    visual.rect.setTextureRect(sf::IntRect(0, 0, visual.spriteW, visual.spriteH));
    visual.rect.setOrigin(sf::Vector2(static_cast<float>(visual.spriteW) * 0.5f, static_cast<float>(visual.spriteH)));
    // posX = visual.rect.getPosition().x;
    // posY = visual.rect.getPosition().y;
    // walkToX = posX;
    // walkToY = posY;
    id = _id;
    hp = maxhp;
}

void character::MoveTo(float _x, float _y, dungeon *currentDungeon)
{
    if (currentDungeon != nullptr)
    {
        _x -= currentDungeon->spawnLocationX;
        _y -= currentDungeon->spawnLocationY;
    }

    // walkToX = _x;
    // walkToY = _y;
}

// void character::strikeTarget(float delta_time)
// {
//     if (!attacking)
//     {
//         attackTimer -= 10.0f * delta_time;
//     }

//     if (attackTimer < 0.0f && playingAnim != ANIM_ABILITY_0 + nextAbility)
//     {
//         PlayAnimation(static_cast<ANIMATION_MAPPINGS>(ANIM_ABILITY_0 + nextAbility), delta_time, false);
//         attacking = true;
//     }
//     if (attacking && animations[ANIM_ABILITY_0 + nextAbility].frame >= _class.abilities_list[nextAbility].activationFrame)
//     {
//         attackTimer = _class.attackSpeed;
//         _class.abilities_list[nextAbility].updateTargets(new character *{target}, 1);
//         _class.abilities_list[nextAbility].use(_class.attackDamage, &_class.abilities_list[0], this);
//         soundplayer.load("../snd/sfx/quickpunch.mp3");
//         soundplayer.play();
//         target->takeHit(delta_time); // this needs to be a reference to the attack->targets, not positional target. Also, attack->targets needs to be able to target anybody within range of the attack, capped at attack->targetcount
//         attacking = false;
//         lastAbility = nextAbility;

//         if (target->hp <= 0 && target->id != id)
//         {
//             experiencePoints += target->_class.maxHP / 10;
//         }
//     }
// }

void character::Update(float delta_time)
{
    // collider.moveCenterToPoint(visual.rect.getPosition().x, visual.rect.getPosition().y + 4.0f);
    collider.min_x = visual.rect.getPosition().x - visual.spriteW * 0.5f;
    collider.max_x = visual.rect.getPosition().x + visual.spriteW * 0.5f;
    collider.min_y = visual.rect.getPosition().y - visual.spriteH;
    collider.max_y = visual.rect.getPosition().y;

    if (hp <= 0)
    {
        visual.rect.setColor(sf::Color(50, 50, 50, 255));
        visual.rect.setRotation(90);
        // visual.Put(posX + screenOffsetX, posY + screenOffsetY);
        // visual.Move(screenOffsetX, screenOffsetY);
        return;
    }

    // float xWalkDist = posX - walkToX;
    // float yWalkDist = posY - walkToY;

    // if (walkToX != posX && walkToY == posY)
    // {
    //     velocityX = -xWalkDist / std::abs(xWalkDist) * runSpeed * delta_time;
    // }
    // if (walkToY != posY && walkToX == posX)
    // {
    //     velocityY = -yWalkDist / std::abs(yWalkDist) * runSpeed * delta_time;
    // }
    // if (walkToX != posX && walkToY != posY)
    // {
    //     float normalizationCap = std::max(std::abs(xWalkDist), std::abs(yWalkDist));
    //     velocityX = -xWalkDist / normalizationCap * runSpeed * delta_time;
    //     velocityY = -yWalkDist / normalizationCap * runSpeed * delta_time;
    // }
    // if (std::abs(xWalkDist) < runSpeed * delta_time * 0.8f)
    //     velocityX = 0.0f;
    // if (std::abs(yWalkDist) < runSpeed * delta_time * 0.8f)
    //     velocityY = 0.0f;

    if (animationFinished)
    {
        PlayAnimation(ANIM_IDLE, delta_time, true);
    }

    animations[playingAnim].run(delta_time, animationLooping);

    // visual.Put(posX + screenOffsetX, posY + screenOffsetY);

    animationFinished = false;

    if (animations[playingAnim].finished)
        animationFinished = true;

    onGround = false;
}
void character::updatePosition(float delta_time)
{
    visual.Move(velocityX * delta_time, velocityY * delta_time);
}

void character::SetAnimation(ANIMATION_MAPPINGS id, unsigned int s, unsigned int e, float spd)
{
    animations[id] = animation(&visual, s, e, spd);
}
void character::PlayAnimation(ANIMATION_MAPPINGS id, float delta_time, bool loops)
{
    if (animations[id]._sprite == nullptr)
        return;

    animations[id].frame = animations[id].start;
    animations[id].timer = 10.0f;
    playingAnim = id;
    animationLooping = loops;
}

int qsPartition(sprite *sprites[entity_limit], int low, int high) // see if there's a better optimized way to do this, such as random pivot
{
    sprite *pivot = sprites[low];

    int i = high + 1;
    for (int j = low + 1; j <= high; ++j)
    {
        if (sprites[j]->rect.getPosition().y > pivot->rect.getPosition().y)
        {
            --i;
            std::swap(sprites[i], sprites[j]);
        }
    }

    std::swap(sprites[i - 1], sprites[low]);
    return i - 1;
}
void quicksortSprites(sprite *sprites[entity_limit], int low, int high)
{
    if (low < high)
    {
        int pi = qsPartition(sprites, low, high);

        quicksortSprites(sprites, low, pi - 1);
        quicksortSprites(sprites, pi + 1, high);
    }
} // quadtree, finishing attack system and targetting, character creation and input to the main player.
void game_system::Add(character *e)
{
    characters[characterCount] = e;
    sortedSprites[characterCount] = &characters[characterCount]->visual;
    ++characterCount;
}
void game_system::Add(particlesystem *p)
{
    particles[particlesystemcount] = p;
    ++particlesystemcount;
}

void game_system::handleMusic()
{
    if (game_music.getDuration() == sf::Time::Zero)
    {
        return;
    }
    if (!music_playing || game_music.getStatus() == sf::Music::Stopped)
    {
        game_music.play();
        music_playing = true;
    }
}

void game_system::update(dungeon &floor, float delta_time)
{
    if (paused)
    {
        for (int i = 0; i < characterCount; ++i)
        {
            characters[i]->visual.Put(characters[i]->visual.rect.getPosition().x, characters[i]->visual.rect.getPosition().y);
        }
        return;
    }

    quicksortSprites(sortedSprites, 0, characterCount - 1);
    for (int i = 0; i < characterCount; ++i)
    {
        for (int j = 0; j < characterCount; ++j)
        {
            if (i == j)
                continue;

            switch (characters[i]->id)
            {
            case CH_PLAYER:
                break;
            case CH_MONSTER:
                break;
            }
        }
        characters[i]->Update(delta_time);
        if (!characters[i]->onGround)
        {
            characters[i]->velocityY += 900.0f * delta_time;
        }

        for (int j = 0; j < floor.collision_box_count; ++j)
        {
            if (floor.collision_boxes[j].min_x == 0.0f &&
                floor.collision_boxes[j].min_y == 0.0f &&
                floor.collision_boxes[j].max_x == 0.0f &&
                floor.collision_boxes[j].max_y == 0.0f)
            {
                continue;
            }

            float xNormal = 0.0f, yNormal = 0.0f;

            float firstCollisionHitTest = characters[i]->collider.response(characters[i]->velocityX * delta_time,
                                                                           characters[i]->velocityY * delta_time, floor.collision_boxes[j], xNormal, yNormal);

            switch (floor.collision_boxes[j].collisionID)
            {
            case 1:
                if (firstCollisionHitTest < 1.0f)
                    characters[i]->hp = 0;
                break;
            case 2:
                if (firstCollisionHitTest < 1.0f && characters[i]->isAPlayer)
                    levelincreasing = true;
                break;
            default:
                if (firstCollisionHitTest < 1.0f && xNormal < 0.0f && characters[i]->velocityX > 0.0f)
                {
                    characters[i]->velocityX *= firstCollisionHitTest;
                }
                if (firstCollisionHitTest < 1.0f && xNormal > 0.0f && characters[i]->velocityX < 0.0f)
                {
                    characters[i]->velocityX *= firstCollisionHitTest;
                }
                if (firstCollisionHitTest < 1.0f && yNormal < 0.0f && characters[i]->velocityY > 0.0f)
                {
                    characters[i]->onGround = true;
                    characters[i]->velocityY *= firstCollisionHitTest;
                }
                if (firstCollisionHitTest < 1.0f && yNormal > 0.0f && characters[i]->velocityY < 0.0f)
                {
                    characters[i]->velocityY *= firstCollisionHitTest;
                }
                break;
            }
        }
        characters[i]->updatePosition(delta_time);
    }

    for (int i = 0; i < particlesystemcount; ++i)
    {
        particles[i]->spawn(delta_time);
        particles[i]->update(delta_time);
    }
}

void game_system::killParticles()
{
    for (int i = 0; i < particlesystemcount; ++i)
    {
        particles[i]->kill();
    }
}