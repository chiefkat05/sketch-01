/** liscense will go here when I stop being lazy **/
/** special thanks to fsuarez913 for my placeholder font :) https://www.dafont.com/super-kinds.font */

#include "../headers/dungeon.h"
#include "../headers/system.h"
#include "../headers/gamestate.h"
#include "../headers/collision.h"

float mouseX, mouseY;
bool mousePressed, mouseClicked, mouseReleased;
float delta_time = 0.0f;
bool buttonHovered = false;

extern gui gui_data;
extern game_state state;
extern connector host;

void dungeonInit(game_system &game, dungeon &dg, std::string tilePath, std::string levelPath);

void playerControl(game_system &game, character &p, sf::RenderWindow &window, dungeon *floor)
{
    p.velocityX = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        // p.MoveTo(p.visual.rect.getPosition().x - 4.0f, p.visual.rect.getPosition().y, floor);
        p.velocityX = -p.runSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        // p.MoveTo(p.visual.rect.getPosition().x + 4.0f, p.visual.rect.getPosition().y, floor);
        p.velocityX = p.runSpeed;
        // p.visual.Move(50.0f * delta_time, 0.0f);
    }
    if (!p.onGround)
    {
        p.jumped = false;
    }
    if (p.onGround && !p.jumped && (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
    {
        // p.MoveTo(p.visual.rect.getPosition().x, p.visual.rect.getPosition().y - 4.0f, floor);
        p.velocityY -= 3.2f * p.runSpeed;
        p.jumped = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        // p.allies[i].MoveTo(p.allies[i].visual.rect.getPosition().x, p.allies[i].visual.rect.getPosition().y + 4.0f, floor);
    }
}

float current_time = 0.0f;
float previous_time = 0.0f;

void mouseUpdate()
{
    mouseClicked = false;
    mouseReleased = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !mousePressed)
    {
        mouseClicked = true;
        mousePressed = true;
    }

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (mousePressed)
            mouseReleased = true;
        mouseClicked = false;
        mousePressed = false;
    }
}

int prevState = -1;
int entityHP[entity_limit];
int entityHP_UI_Index = 0;
// edit all guis here
void menuData(game_system &mainG, character &mainP, dungeon &floor)
{
    if (state == prevState)
        return;

    gui_data.elements.clear();

    std::string temp_path;
    mainG.killParticles();
    particlesystem snowparticles;
    switch (state)
    {
    case START_SCREEN:
        if (!mainG.game_music.openFromFile("../snd/mus/M-01.mp3"))
        {
            std::cout << "failed to load ../snd/mus/M-01.mp3\n";
        }
        gui_data.background = sprite("../img/pond.png", 0.0f, 0.0f, 2, 5);
        gui_data.bgAnim = animation(&gui_data.background, 0, 9, 50.0f);
        gui_data.background.rect.setColor(sf::Color(255, 255, 255, 255));
        gui_data.elements.push_back(ui_element(UI_CLICKABLE, "../img/p.png", 10.0f, 50.0f, 4, 1, startGame, nullptr, nullptr, nullptr, CHARACTER_CREATION_SCREEN));
        gui_data.elements[gui_data.elements.size() - 1].anim = animation(&gui_data.elements[gui_data.elements.size() - 1].visual, 0, 3, 180.0f);
        gui_data.elements[gui_data.elements.size() - 1].anim.run(delta_time, true);
        // gui_data.elements.push_back(ui_element(UI_CLICKABLE, "../img/s.png", 80.0f, 50.0f, 8, 1, optionsTab, nullptr, &mainG));
        // gui_data.elements[gui_data.elements.size() - 1].anim = animation(&gui_data.elements[gui_data.elements.size() - 1].visual, 0, 7, 180.0f);
        // gui_data.elements[gui_data.elements.size() - 1].anim.run(delta_time, true);
        // gui_data.elements.push_back(ui_element(UI_CLICKABLE, "../img/q.png", 100.0f, 50.0f, 4, 4, quitGame));
        // gui_data.elements[gui_data.elements.size() - 1].anim = animation(&gui_data.elements[gui_data.elements.size() - 1].visual, 0, 12, 180.0f);
        // gui_data.elements[gui_data.elements.size() - 1].anim.run(delta_time, true);
        break;
    case MENU_SCREEN:
        // floor.screenPositionX = 0.0f;
        // floor.screenPositionY = 0.0f;
        gui_data.background = sprite("../img/particles.png", 0.0f, 0.0f, 3, 1);
        gui_data.background.rect.setColor(sf::Color(255, 255, 255, 255));
        gui_data.elements.push_back(ui_element(UI_CLICKABLE, "../img/p.png", 40.0f, 40.0f, 1, 1,
                                               startGame, nullptr, nullptr, nullptr, prevState));
        gui_data.elements[gui_data.elements.size() - 1].anim = animation(&gui_data.elements[gui_data.elements.size() - 1].visual, 0, 3, 180.0f);
        break;
    case CHARACTER_CREATION_SCREEN:
        gui_data.background = sprite("../img/c_s.png", 0.0f, 0.0f, 1, 1);
        gui_data.background.rect.setColor(sf::Color(255, 255, 255, 255));
        gui_data.elements.push_back(ui_element(UI_CLICKABLE, "../img/p.png", 40.0f, 40.0f, 1, 1, startGame, nullptr, nullptr, nullptr, DUNGEON_SCREEN));
        break;
    case DUNGEON_SCREEN:
        if (mainG.levelincreasing)
        {
            mainG.levelincreasing = false;
            mainG.level++;
        }
        switch (mainG.level)
        {
        case 0:
            dungeonInit(mainG, floor, "../img/01-tiles.png", "../dungeons/L01.sdf");
            break;
        case 1:
            dungeonInit(mainG, floor, "../img/01-tiles.png", "../dungeons/L02.sdf");
            break;
        case 2:
            dungeonInit(mainG, floor, "../img/01-tiles.png", "../dungeons/L03.sdf");
            break;
        case 3:
            dungeonInit(mainG, floor, "../img/01-tiles.png", "../dungeons/W.sdf");
            break;
        case 4:
            state = MENU_SCREEN;
            return;
        default:
            std::cout << ":megamind: no level?\n";
            dungeonInit(mainG, floor, "../img/01-tiles.png", "../dungeons/L01.sdf");
            break;
        }

        if (!mainG.game_music.openFromFile("../snd/mus/L-04.mp3"))
        {
            std::cout << "failed to load ../snd/mus/L-04.mp3\n";
        }
        gui_data.background = sprite("../img/01.png", 0.0f, 0.0f, 1, 2);
        // gui_data.background.rect.setColor(sf::Color(255, 255, 255, 255));
        gui_data.bgAnim = animation(&gui_data.background, 0, 1, 50.0f);
        gui_data.bgAnim.setScale(2.0f, 1.0f);

        mainP.visual.Put(floor.spawnLocationX, floor.spawnLocationY);
        // gui_data.elements.push_back(ui_element(UI_CLICKABLE, "../img/s.png", 218.0f, 102.0f, 1, 1, optionsTab, nullptr, &mainG));

        // if (prevState == CHARACTER_CREATION_SCREEN)
        // {
        //     mainP.posX = floor.spawnLocationX;
        //     mainP.posY = floor.spawnLocationY;
        //     mainP.walkToX = floor.spawnLocationX;
        //     mainP.walkToY = floor.spawnLocationY;
        // }

        break;
    case WON_LEVEL_STATE:
        std::cout << "wow\n";
        if (!mainG.levelincreasing)
        {
            std::cout << "how did you do this\n";
        }
        break;
    default:
        break;
    }

    prevState = state;
}

bool pauseKeyHeld = false, uiKeyHeld = false, showUI = true;
void playerInit(character &pl, game_system &game)
{
    pl = character("../img/stick.png", 120.0f, 40.0f, 4, 1, CH_PLAYER);
    pl.isAPlayer = true;

    pl.SetAnimation(ANIM_IDLE, 0, 0, 0.0f);
    pl.SetAnimation(ANIM_WALK, 0, 1, 150.0f);
    pl.SetAnimation(ANIM_ABILITY_0, 2, 2, 0.0f);
    pl.SetAnimation(ANIM_ABILITY_1, 3, 3, 0.0f);

    game.Add(&pl);
}
void dungeonInit(game_system &game, dungeon &dg, std::string tilePath, std::string levelPath)
{
    dg = dungeon(tilePath.c_str());
    dg.readRoomFile(levelPath.c_str());
}

extern game_system game;
extern bool typingInput;
int main()
{
    sf::RenderWindow window(sf::VideoMode(256, 128), "sketch-01");
    sf::View screen(sf::FloatRect(0.0f, 0.0f, 256.0f, 128.0f));

    window.setView(screen);

    sf::Clock sfClock;
    sf::Time sfTime;

    dungeon mainDungeon;
    character mainPlayer;
    playerInit(mainPlayer, game);

    sprite resolutionBlinderTop("../img/ui/resolution_blinder.png", 0.0f, 0.0f, 1, 1);
    sprite resolutionBlinderBottom("../img/ui/resolution_blinder.png", 0.0f, 0.0f, 1, 1);

    sf::Font defaultFont;
    defaultFont.setSmooth(false);
    if (!defaultFont.loadFromFile("../img/fonts/Super Kinds.ttf"))
    {
        std::cout << "super kinds font failed to load :(\n";
    }

    while (window.isOpen())
    {
        sf::Vector2i mouseIPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(mouseIPos);
        mouseX = mousePos.x;
        mouseY = mousePos.y;

        sfTime = sfClock.restart();
        delta_time = sfTime.asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                float ratio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
                screen.setSize(256.0f, 256.0f / ratio);
                window.setView(screen);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }

        mouseUpdate();

        if (!pauseKeyHeld && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            game.paused = !game.paused;
        }
        pauseKeyHeld = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            pauseKeyHeld = true;

        window.clear();
        if (state == WON_LEVEL_STATE && game.levelincreasing)
        {
            prevState = WON_LEVEL_STATE;
            state = DUNGEON_SCREEN;
        }
        menuData(game, mainPlayer, mainDungeon);

        game.handleMusic();

        window.draw(gui_data.background.rect);
        if (state == DUNGEON_SCREEN && mainDungeon.dungeonInitialized)
        {
            mainDungeon.changeScreenViewPosition(screen, mainPlayer.visual.rect.getPosition().x, mainPlayer.visual.rect.getPosition().y);
            window.setView(screen);

            mainDungeon.draw(&window);
            game.update(mainDungeon, delta_time);
            playerControl(game, mainPlayer, window, &mainDungeon);

            for (int i = 0; i < game.characterCount; ++i)
            {
                if (game.characters[i]->visual.empty)
                    continue;

                window.draw(game.characters[i]->visual.rect);
            }

            if (mainPlayer.hp <= 0)
            {
                mainPlayer.visual.Put(mainDungeon.spawnLocationX, mainDungeon.spawnLocationY);
                mainPlayer.hp = mainPlayer.maxhp;
            }

            if (game.levelincreasing)
            {
                state = WON_LEVEL_STATE;
            }
        }
        for (int i = 0; i < game.particlesystemcount; ++i)
        {
            game.particles[i]->draw(window, delta_time);
        }
        if (!uiKeyHeld && sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
        {
            showUI = !showUI;
        }
        uiKeyHeld = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
        {
            uiKeyHeld = true;
        }
        if (showUI)
        {
            gui_data.screenDraw(&window, mouseX, mouseY, mousePressed, mouseReleased, delta_time);
        }

        window.display();
    }

    return 0;
}