#include <memory>
#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include "RandomHelpers.h"

const float Game::PlayerSpeed = 100.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
const std::string BlockTexturePath = "../Media/Textures/Block.png";
const std::string LadderTexturePath = "../Media/Textures/Echelle.PNG";
const std::string MarioTexturePath = "../Media/Textures/Mario_small_transparent.png";
const std::string StatisticsFontPath = "../Media/Sansation.ttf";
const std::string CoinTexturePath = "../Media/Textures/coin.png";
const std::string ScoreFontPath = "../Media/BlockyLettersHollow.ttf";

Game::Game() :
    mWindow(sf::VideoMode(1024, 770), "Donkey Kong 1981", sf::Style::Close),
    mTexture(),
    mPlayer(),
    mFont(),
    scoreFont(),
    scoreAnnouncementText(),
    scoreText(),
    mStatisticsText(),
    mStatisticsUpdateTime(),
    mStatisticsNumFrames(0),
    mIsMovingUp(false),
    mIsMovingDown(false),
    mIsMovingRight(false),
    mIsMovingLeft(false)
{
    mWindow.setFramerateLimit(160);
    score = 0;

    drawBlocks();
    drawLadders();
    drawCoins();
    drawMario();
    drawStatistics();
    drawScore();
}

void Game::drawBlocks() {
    _TextureBlock.loadFromFile(BlockTexturePath);
    _sizeBlock = _TextureBlock.getSize();

    for (int i = 0; i < BASE_BLOCK_COUNT; i++) {
        _Block[i][BLOCK_COUNT_Y].setTexture(_TextureBlock);

        _Block[i][BLOCK_COUNT_Y].setPosition(-70.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (BLOCK_COUNT_Y + 1));

        std::shared_ptr<Entity> se = std::make_shared<Entity>();
        se->m_sprite = _Block[i][BLOCK_COUNT_Y];
        se->m_type = EntityType::block;
        se->m_size = _TextureBlock.getSize();
        se->m_position = _Block[i][BLOCK_COUNT_Y].getPosition();
        EntityManager::m_Entities.push_back(se);
    }

    for (int i = 0; i < BLOCK_COUNT_X; i++) {
        for (int j = 0; j < BLOCK_COUNT_Y; j++) {
            _Block[i][j].setTexture(_TextureBlock);

            if (j%  2) {
                _Block[i][j].setPosition(30.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (j + 1));
            } else {
                _Block[i][j].setPosition(90.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (j + 1));
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

    for (int i = 0; i <= SCALE_COUNT; i++) {
        _Ladder[i].setTexture(_LadderTexture);
        if(i % 2){
            _Ladder[i].setPosition(750.f + 70.f, 0.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y);
        }
        else {
            _Ladder[i].setPosition(120.f + 70.f, 0.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y);
        }


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

void Game::drawScore() {
    scoreFont.loadFromFile(ScoreFontPath);

    scoreAnnouncementText.setString("Score");
    scoreAnnouncementText.setFont(scoreFont);
    scoreAnnouncementText.setPosition(680.f, 5.f);
    scoreAnnouncementText.setCharacterSize(40);

    scoreText.setString(std::to_string(score));
    scoreText.setFont(scoreFont);
    scoreText.setPosition(680.f, 50.f);
    scoreText.setCharacterSize(22);
}

void Game::drawCoins() {
    _CoinTexture.loadFromFile(CoinTexturePath);

    for (int i = 0; i < COIN_COUNT; i++) {
        _Coin[i].setTexture(_CoinTexture);

        // Get random block, then put a coin upon it
        int blockX = getRandomNumber(0, BLOCK_COUNT_X);
        int blockY = getRandomNumber(0, BLOCK_COUNT_Y);

        sf::Sprite randomBlock = _Block[blockX][blockY];

        _Coin[i].setPosition(
            randomBlock.getPosition().x,
            0.f + BLOCK_SPACE * i + 2 * _sizeBlock.y
        );

        std::shared_ptr<Entity> se = std::make_shared<Entity>();
        se->m_sprite = _Coin[i];
        se->m_type = EntityType::coin;
        se->m_size = _CoinTexture.getSize();
        se->m_position = _Coin[i].getPosition();
        EntityManager::m_Entities.push_back(se);
    }
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
            handleCoins();
            update(TimePerFrame);
        }

        updateScore();
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

    mWindow.draw(scoreAnnouncementText);
    mWindow.draw(scoreText);
    mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Game::updateScore() {
    scoreText.setString(std::to_string(score));
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

void Game::handleCoins() {
    auto playerBounds = EntityManager::GetPlayer().get()->m_sprite.getGlobalBounds();
    auto coins = EntityManager::GetCoins();

    for (auto const& coin: coins) {
        auto coinGlobalBounds = coin.get()->m_sprite.getGlobalBounds();

        if (playerBounds.intersects(coinGlobalBounds)) {
            EntityManager::RemoveCoin(coin);
            score += COIN_VALUE;
        }
    }
}
