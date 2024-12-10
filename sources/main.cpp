/** liscense will go here when I stop being lazy **/
/** special thanks to fsuarez913 for my placeholder font :) https://www.dafont.com/super-kinds.font */

#include "../headers/dungeon.h"
#include "../headers/system.h"
#include "../headers/gamestate.h"
#include "../headers/collision.h"

float mouseX, mouseY;
bool mousePressed, mouseClicked, mouseReleased;
float massScale = 1.0f, massYOffset = 0.0f;
float delta_time = 0.0f;
bool buttonHovered = false;

extern gui gui_data;
extern game_state state;
extern connector host;

void dungeonInit(game_system &game, dungeon &dg);

void playerControl(game_system &game, character &p, sf::RenderWindow &window, dungeon *floor)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        // p.MoveTo(p.visual.rect.getPosition().x - 4.0f * massScale, p.visual.rect.getPosition().y, floor);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        // p.MoveTo(p.visual.rect.getPosition().x + 4.0f * massScale, p.visual.rect.getPosition().y, floor);
    }
    if (!p.jumped && (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
    {
        // p.MoveTo(p.visual.rect.getPosition().x, p.visual.rect.getPosition().y - 4.0f * massScale, floor);
        p.onGround = false;
        p.jumped = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        // p.allies[i].MoveTo(p.allies[i].visual.rect.getPosition().x, p.allies[i].visual.rect.getPosition().y + 4.0f * massScale, floor);
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
        gui_data.elements.push_back(ui_element(UI_CLICKABLE, "../img/p.png", 60.0f, 50.0f, 4, 1, startGame, nullptr, nullptr, nullptr, CHARACTER_CREATION_SCREEN));
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
        floor.screenPositionX = 0.0f;
        floor.screenPositionY = 0.0f;
        gui_data.background = sprite("../img/particles.png", 0.0f, 0.0f, 3, 1);
        gui_data.background.rect.setColor(sf::Color(255, 255, 255, 255));
        gui_data.elements.push_back(ui_element(UI_CLICKABLE, "../img/p.png", 40.0f, 40.0f, 1, 1,
                                               startGame, nullptr, nullptr, nullptr, prevState));
        break;
    case CHARACTER_CREATION_SCREEN:
        gui_data.background = sprite("../img/c_s.png", 0.0f, 0.0f, 1, 1);
        gui_data.background.rect.setColor(sf::Color(255, 255, 255, 255));
        gui_data.elements.push_back(ui_element(UI_CLICKABLE, "../img/p.png", 40.0f, 40.0f, 1, 1, startGame, nullptr, nullptr, nullptr, DUNGEON_SCREEN));
        break;
    case DUNGEON_SCREEN:
        dungeonInit(mainG, floor);

        if (!mainG.game_music.openFromFile("../snd/mus/L-04.mp3"))
        {
            std::cout << "failed to load ../snd/mus/L-04.mp3\n";
        }
        gui_data.background = sprite("../img/01.png", 0.0f, 0.0f, 1, 2);
        // gui_data.background.rect.setColor(sf::Color(255, 255, 255, 255));
        // gui_data.background.rect.setTextureRect(sf::IntRect(0, 0, 512, 64));
        gui_data.bgAnim = animation(&gui_data.background, 0, 1, 50.0f);
        // gui_data.elements.push_back(ui_element(UI_CLICKABLE, "../img/s.png", 218.0f, 102.0f, 1, 1, optionsTab, nullptr, &mainG));

        if (prevState == CHARACTER_CREATION_SCREEN)
        {
            mainP.posX = floor.spawnLocationX;
            mainP.posY = floor.spawnLocationY;
            mainP.walkToX = floor.spawnLocationX;
            mainP.walkToY = floor.spawnLocationY;
        }

        break;
    case LOSE_SCREEN:
        break;
    case WIN_SCREEN:
        if (!mainG.game_music.openFromFile("../snd/mus/V-L.mp3"))
        {
            std::cout << "failed to load ../snd/mus/V-L.mp3\n";
        }
        floor.screenPositionX = 64.0f;
        floor.screenPositionY = 0.0f;
        gui_data.background = sprite("../img/ui/backgrounds/wave.png", 0.0f, 0.0f, 1, 7);
        gui_data.elements.push_back(ui_element(UI_IMAGE, "../img/s.png", 128.0f, 24.0f, 1, 1, nullptr));

        // gui_data.elements.push_back(ui_element(UI_IMAGE, "../img/ui/victory.png", 0.0f, 0.0f, 256.0f, 128.0f, 1, 1, nullFunc));
        // load victory map here
        break;
    default:
        break;
    }

    prevState = state;
}

bool pauseKeyHeld = false, uiKeyHeld = false, showUI = true;
void playerInit(character &pl, game_system &game)
{
    pl = character("../img/stick.png", 120.0f, 40.0f, 1, 4, CH_PLAYER);

    pl.SetAnimation(ANIM_IDLE, 0, 0, 0.0f);
    pl.SetAnimation(ANIM_WALK, 0, 1, 150.0f);
    pl.SetAnimation(ANIM_ABILITY_0, 2, 2, 0.0f);
    pl.SetAnimation(ANIM_ABILITY_1, 3, 3, 0.0f);

    game.Add(&pl);
}
void dungeonInit(game_system &game, dungeon &dg)
{
    dg = dungeon("../img/01-tiles.png", 64.0f, 64.0f, massScale, massYOffset); // in case you were wondering, the dungeon sprite is empty and that's why you can't see it :)
    dg.readRoomFile("../dungeons/L01.sdf", massScale, massYOffset);
    // particlesystem snowparticles("../img/particles/snow.png", 2.0f, 2.0f, 0, 0, 4, 4, 9999);
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

    sprite resolutionBlinderTop("../img/ui/resolution_blinder.png", 0.0f, massYOffset * massScale, 1, 1);
    sprite resolutionBlinderBottom("../img/ui/resolution_blinder.png", 0.0f, massYOffset * massScale, 1, 1);

    sf::Font defaultFont;
    defaultFont.setSmooth(false);
    if (!defaultFont.loadFromFile("../img/fonts/Super Kinds.ttf"))
    {
        std::cout << "super kinds font failed to load :(\n";
    }

    sf::Event event; // why segmentation fault on any event???
    while (window.isOpen())
    {
        mouseX = sf::Mouse::getPosition(window).x;
        mouseY = sf::Mouse::getPosition(window).y;

        previous_time = current_time;
        sfTime = sfClock.getElapsedTime();
        current_time = sfTime.asSeconds();
        delta_time = current_time - previous_time;

        std::cout << "crash?\n";
        while (window.pollEvent(event))
        {
            std::cout << "fix inside?\n";
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                float ratio = static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y);
                massScale = 256.0f / static_cast<float>(window.getSize().x);
                massYOffset = (1 - ratio * 0.5f) * static_cast<float>(window.getSize().y * massScale * 0.5f);
                screen.setSize(256.0f, 256.0f / ratio);
                window.setView(screen);
            }
            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "???\n";
            }
        }
        std::cout << "here?\n";

        mouseUpdate();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        if (!pauseKeyHeld && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            game.paused = !game.paused;
        }
        pauseKeyHeld = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            pauseKeyHeld = true;

        window.clear();

        menuData(game, mainPlayer, mainDungeon); // player position thing next    // ???
        game.handleMusic();

        gui_data.background.rect.setPosition(mainDungeon.screenPositionX + gui_data.background.walkToX, mainDungeon.screenPositionY + gui_data.background.walkToY);
        window.draw(gui_data.background.rect);
        if (state == DUNGEON_SCREEN && mainDungeon.dungeonInitialized)
        {
            mainDungeon.updateScreenPosition(100.0f, 100.0f, delta_time, massScale, massYOffset);

            mainDungeon.draw(&window);
            game.update(mainDungeon, delta_time);
            // playerControl(game, mainPlayer, window, &mainDungeon);

            for (int i = 0; i < game.characterCount; ++i)
            {
                if (game.characters[i]->visual.empty)
                    continue;

                window.draw(game.characters[i]->visual.rect);
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

        // window.draw(textInput);

        resolutionBlinderTop.Put(0.0f, -massYOffset / massScale);
        resolutionBlinderTop.rect.setScale(256.0f, massYOffset / massScale / resolutionBlinderTop.spriteH);
        window.draw(resolutionBlinderTop.rect);
        resolutionBlinderBottom.Put(0.0f, 128.0f);
        resolutionBlinderBottom.rect.setScale(256.0f, massYOffset * massScale);
        window.draw(resolutionBlinderBottom.rect);

        window.display();
    }

    return 0;
}

// okay since for some reason my notes disappeared

// particle system needs everything complete in the creation function so that game_system can update it and dev can forget after placing the function
// particle systems also need to self-delete after the last particle has run out of life
// which means you will need to look into the strange bug where particles will stay stuck forever
// and there should be an extra integer value on the particle spawn function for how many particles to spawn over lifetime before stopping completely
// 0 is infinite

// multiplayer should be somewhat straightforward, start by seeing if the sprite spawning works on 127.0.0.1 (make it spawn in through the menuData function)
// once sprite spawning is working, you need to think up a more valid form of sprite identification than just the id.
// (Or figure that out, since it would be pretty much foolproof, just stupid in concept)
// you don't want to use pointers, since that would be retarded.
// you could use identification strings, but that might make the code messy
// maybe try it on a new branch?
// don't use positional coordinates, that would be retarded
// after you get a sprite to appear and can identify it, moving it should be as one position update message away

// quadtree
// no need to deal with it since there's literally 5 objects in this one, do that next game

// sound effects should not be hard, but look up the sfml tutorial if you don't want to actually do the thinking yourself
// make a simple pre-loaded list in the soundhandler struct
// load sound into list function
// play sound from list index function
// good job it works now
// oh wait you don't even have sound effects
// record yourself stomping and give it echo or something idk
// or use zyn for stylization and ease of design

// at some point, you'll need to add objects.
// so you'll need to make art
// here's some ideas for objects you can work on while bored or not wanting to do any coding
// balloon
// car ? maybe moving car on road
// person behind tree
// hey maybe you should seperate the trees from the bg screen and make them their own object
// snowman
// campfire
// the road lines?
// street lamp
// balloon
// snake (fish?) crossing sign
// deer
// key
// chest
// clown
// handgun
// closet to hide in
// flying fish
// cat
// dog
// yourself
// upside down tree
// health pack
// rats (with big rat)
// empty trash bin
// man in a pot with a hammer
// d20
// hat
// elevator
// headphones
// house ? cabin ?
// some musical instrument
// gramophone ?
// basket
// sharpie
// notebook
// letters on trees
// crooked man
// e-man (flute)
// phone booth ?
// skeleton (model ?)
// map of place that doesn't exist
// flashlight
// spider friend
// purple beetle
// tesla gate ?
// generator
// floating cubes in sky
// bell
// large crack in road