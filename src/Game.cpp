#include <memory>
#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"

const float Game::PlayerSpeed = 100.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
const std::string BlockTexturePath = "../Media/Textures/Block.png";
const std::string LadderTexturePath = "../Media/Textures/Echelle.PNG";
const std::string MarioTexturePath = "../Media/Textures/Mario_small_transparent.png";
const std::string StatisticsFontPath = "../Media/Sansation.ttf";

Game::Game() :
    mWindow(sf::VideoMode(840, 600), "Donkey Kong 1981", sf::Style::Close),
    mTexture(),
    mPlayer(),
    mFont(),
    mStatisticsText(),
    mStatisticsUpdateTime(),
    mStatisticsNumFrames(0),
    mIsMovingUp(false),
    mIsMovingDown(false),
    mIsMovingRight(false),
    mIsMovingLeft(false)
{
    mWindow.setFramerateLimit(160);
    drawBlocks();
    drawLadders();
    drawMario();
    drawStatistics();
}

void Game::drawBlocks() {
    _TextureBlock.loadFromFile(BlockTexturePath);
    _sizeBlock = _TextureBlock.getSize();

    for (int i = 0; i < BLOCK_COUNT_X; i++) {
        for (int j = 0; j < BLOCK_COUNT_Y; j++) {
            _Block[i][j].setTexture(_TextureBlock);

            if (j%  2) {
                _Block[i][j].setPosition(-30.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (j + 1));
            } else {
                _Block[i][j].setPosition(80.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (j + 1));
            }

            std::shared_ptr<Entity> se = std::make_shared<Entity>();
            se->m_sprite = _Block[i][j];
            se->m_type = EntityType::block;
            se->m_size = _TextureBlock.getSize();
            se->m_position = _Block[i][j].getPosition();
            EntityManager::m_Entities.push_back(se);
        }
    }
}

void Game::drawLadders() {
    _LadderTexture.loadFromFile(LadderTexturePath);

    for (int i = 0; i < SCALE_COUNT; i++) {
        _Ladder[i].setTexture(_LadderTexture);
        _Ladder[i].setPosition(100.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y);

        std::shared_ptr<Entity> se = std::make_shared<Entity>();
        se->m_sprite = _Ladder[i];
        se->m_type = EntityType::scale;
        se->m_size = _LadderTexture.getSize();
        se->m_position = _Ladder[i].getPosition();
        EntityManager::m_Entities.push_back(se);
    }
}

void Game::drawMario() {
    mTexture.loadFromFile(MarioTexturePath);
    _sizeMario = mTexture.getSize();
    mPlayer.setTexture(mTexture);
    sf::Vector2f posMario;
    posMario.x = 100.f + 70.f;
    posMario.y = BLOCK_SPACE * 5 - _sizeMario.y;

    mPlayer.setPosition(posMario);

    std::shared_ptr<Entity> player = std::make_shared<Entity>();
    player->m_sprite = mPlayer;
    player->m_type = EntityType::player;
    player->m_size = mTexture.getSize();
    player->m_position = mPlayer.getPosition();
    EntityManager::m_Entities.push_back(player);
}

void Game::drawStatistics() {
    mFont.loadFromFile(StatisticsFontPath);
    mStatisticsText.setString("Welcome to Donkey Kong 1981");
    mStatisticsText.setFont(mFont);
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(10);
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }

        updateStatistics(elapsedTime);
        render();
    }
}

void Game::processEvents(){
    sf::Event event{};

    while (mWindow.pollEvent(event)){
        switch (event.type){
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code, true);
                break;

            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;

            case sf::Event::Closed:
                mWindow.close();
                break;

            default: ;
        }
    }
}

void Game::update(sf::Time elapsedTime){
    sf::Vector2f movement(0.f, 0.f);
    if (mIsMovingUp)
        movement.y -= PlayerSpeed;
    if (mIsMovingDown)
        movement.y += PlayerSpeed;
    if (mIsMovingLeft)
        movement.x -= PlayerSpeed;
    if (mIsMovingRight)
        movement.x += PlayerSpeed;

    for (const std::shared_ptr<Entity> &entity : EntityManager::m_Entities){
        if (!entity->m_enabled || entity->m_type != EntityType::player){
            continue;
        }

        entity->m_sprite.move(movement * elapsedTime.asSeconds());
    }
}

void Game::render(){
    mWindow.clear();

    for (const std::shared_ptr<Entity> &entity : EntityManager::m_Entities){
        if (!entity->m_enabled){
            continue;
        }

        mWindow.draw(entity->m_sprite);
    }

    mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime) {
    mStatisticsUpdateTime += elapsedTime;
    mStatisticsNumFrames += 1;

    if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
        mStatisticsText.setString(
                "Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
                "Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }

    // Handle collision
    if (mStatisticsUpdateTime >= sf::seconds(0.050f)) {
        // Handle collision weapon enemies
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    switch (key) {
        case sf::Keyboard::Up:
            mIsMovingUp = isPressed;
            break;
        case sf::Keyboard::Down:
            mIsMovingDown = isPressed;
            break;
        case sf::Keyboard::Left:
            mIsMovingLeft = isPressed;
            break;
        case sf::Keyboard::Right:
            mIsMovingRight = isPressed;
            break;

        // TODO: Jump!!!
        case sf::Keyboard::Space:
            break;

        default: ;
    }
}
