#include <memory>
#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include "RandomHelpers.h"
#include "Animation.h"
#include "AnimatedSprite.h"

const float Game::PlayerSpeed = 100.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
const std::string BlockTexturePath = "../Media/Textures/Block.png";
const std::string LadderTexturePath = "../Media/Textures/Echelle.PNG";
const std::string MarioSpriteSheetPath = "../Media/Textures/mario_sprite.png";
const std::string StatisticsFontPath = "../Media/Sansation.ttf";
const std::string CoinTexturePath = "../Media/Textures/coin.png";
const std::string ScoreFontPath = "../Media/BlockyLettersHollow.ttf";

Game::Game() :
    mWindow(sf::VideoMode(SCREEN_HEIGHT, SCREEN_WIDTH), "Donkey Kong 1981", sf::Style::Close),
    mSpriteSheet(),
    mFont(),
    scoreFont(),
    scoreAnnouncementText(),
    scoreText(),
    mStatisticsText(),
    mStatisticsUpdateTime(),
    mStatisticsNumFrames(0),
    score(0),
    debug(true)
{
    mWindow.setFramerateLimit(160);
    mario = std::make_shared<Mario>();
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

    int up_base = 0;

    for (int i = 0; i < BASE_BLOCK_COUNT; i++) {
        _Block[i][BLOCK_COUNT_Y].setTexture(_TextureBlock);

        if (i < (BASE_BLOCK_COUNT / 2)) {
            _Block[i][BLOCK_COUNT_Y].setPosition(-70.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (BLOCK_COUNT_Y + 1));
        } else {
            up_base += 1;
            _Block[i][BLOCK_COUNT_Y].setPosition(-70.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (BLOCK_COUNT_Y + 1) - up_base);
        }

        std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::block);
        se->m_sprite = _Block[i][BLOCK_COUNT_Y];
        se->m_size = _TextureBlock.getSize();
        se->m_position = _Block[i][BLOCK_COUNT_Y].getPosition();
        EntityManager::m_Entities.push_back(se);
    }

    for (int i = 0; i < BLOCK_COUNT_X; i++) {
        for (int j = 0; j < BLOCK_COUNT_Y; j++) {
            _Block[i][j].setTexture(_TextureBlock);

            if (j % 2) {
                _Block[i][j].setPosition(130.f + 70.f * (i + 1),-5.f + BLOCK_SPACE * (j + 1) + (i + 1));
            } else {
                _Block[i][j].setPosition(190.f + 70.f * (i + 1), 5.f + BLOCK_SPACE * (j + 1) - (i + 1));
            }

            std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::block);
            se->m_sprite = _Block[i][j];
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

        if (i % 2){
            _Ladder[i].setPosition(830.f + 70.f, -30.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y);
        } else {
            _Ladder[i].setPosition(230.f + 70.f, -30.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y);
        }

        std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::scale);
        se->m_sprite = _Ladder[i];
        se->m_size = _LadderTexture.getSize();
        se->m_position = _Ladder[i].getPosition();
        EntityManager::m_Entities.push_back(se);
    }
}

void Game::drawMario() {
    mSpriteSheet.loadFromFile(MarioSpriteSheetPath);

    const int FRAME_WIDTH = 16;
    const int FRAME_HEIGHT = 16;
    const float SCALE_WIDTH = MARIO_WIDTH / FRAME_WIDTH;
    const float SCALE_HEIGHT = MARIO_HEIGHT / FRAME_HEIGHT;

    // Setup Mario
    mario->m_size = sf::Vector2u(MARIO_WIDTH, MARIO_HEIGHT);
    mario->m_position = sf::Vector2f(100.f + 70.f, BLOCK_SPACE * 7 - MARIO_HEIGHT);

    // Static sprites
    auto standingRightSprite = sf::IntRect(162, 0, FRAME_WIDTH, FRAME_HEIGHT);
    auto standingLeftSprite = sf::IntRect(122, 0, FRAME_WIDTH, FRAME_HEIGHT);
    mario->standingRightRect = sf::Sprite(mSpriteSheet, standingRightSprite);
    mario->standingLeftRect = sf::Sprite(mSpriteSheet, standingLeftSprite);
    mario->m_sprite = mario->standingRightRect;

    mario->standingLeftRect.scale(SCALE_WIDTH, SCALE_HEIGHT);
    mario->standingRightRect.scale(SCALE_WIDTH, SCALE_HEIGHT);

    // Animations
    mario->walkingAnimationRight.setSpriteSheet(mSpriteSheet);
    mario->walkingAnimationRight.addFrame(standingRightSprite);
    mario->walkingAnimationRight.addFrame(sf::IntRect(200, 0, FRAME_WIDTH, FRAME_HEIGHT));
    mario->walkingAnimationRight.addFrame(sf::IntRect(240, 0, FRAME_WIDTH, FRAME_HEIGHT));

    mario->walkingAnimationLeft.setSpriteSheet(mSpriteSheet);
    mario->walkingAnimationLeft.addFrame(standingLeftSprite);
    mario->walkingAnimationLeft.addFrame(sf::IntRect(80, 0, FRAME_WIDTH, FRAME_HEIGHT));
    mario->walkingAnimationLeft.addFrame(sf::IntRect(40, 0, FRAME_WIDTH, FRAME_HEIGHT));

    mario->currentAnimation = &mario->walkingAnimationRight;

    // set up AnimatedSprite
    auto animatedSprite = AnimatedSprite(sf::seconds(0.1f), true, false);
    animatedSprite.scale(SCALE_WIDTH, SCALE_HEIGHT);
    mario->animatedSprite = animatedSprite;

    // Add Mario to entities
    EntityManager::m_Entities.push_back(mario);
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
    scoreAnnouncementText.setPosition(1080.f, 5.f);
    scoreAnnouncementText.setCharacterSize(40);

    scoreText.setString(std::to_string(score));
    scoreText.setFont(scoreFont);
    scoreText.setPosition(1080.f, 50.f);
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

        std::shared_ptr<Entity> se = std::make_shared<Entity>(false, EntityType::coin);
        se->m_sprite = _Coin[i];
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

    if (mario->isMovingUp) {
        movement.y -= PlayerSpeed;
    }

    if (mario->isMovingDown) {
        movement.y += PlayerSpeed;
    }

    if (mario->isMovingLeft) {
        movement.x -= PlayerSpeed;
        mario->currentAnimation = &mario->walkingAnimationLeft;
        mario->isFacingLeft = true;
        mario->isFacingRight = false;
    }

    if (mario->isMovingRight) {
        movement.x += PlayerSpeed;
        mario->currentAnimation = &mario->walkingAnimationRight;
        mario->isFacingRight = true;
        mario->isFacingLeft = false;
    }

    mario->m_position.x += movement.x * elapsedTime.asSeconds();
    mario->m_position.y += movement.y * elapsedTime.asSeconds();

    mario->animatedSprite.play(*mario->currentAnimation);
    mario->animatedSprite.setPosition(mario->m_position);
    mario->animatedSprite.update(elapsedTime);

    if (!mario->isMoving) {
        mario->animatedSprite.stop();

        if (mario->isFacingLeft) {
            mario->m_sprite = mario->standingLeftRect;
        } else {
            mario->m_sprite = mario->standingRightRect;
        }

        mario->m_sprite.setPosition(mario->m_position);
    }
}

void Game::render(){
    mWindow.clear();

    for (const std::shared_ptr<Entity> &entity : EntityManager::m_Entities) {
        if (entity->isAnimated && entity->isMoving) {
            mWindow.draw(entity->animatedSprite);
        } else {
            mWindow.draw(entity->m_sprite);
        }
    }

    if (debug) {
        sf::RectangleShape marioBox;
        marioBox.setPosition(mario->m_position);
        marioBox.setSize(sf::Vector2f(mario->m_size.x, mario->m_size.y));
        marioBox.setOutlineColor(sf::Color::Red);
        marioBox.setFillColor(sf::Color::Transparent);
        marioBox.setOutlineThickness(2.f);
        mWindow.draw(marioBox);
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
            mario->isMovingUp = mario->isMoving = isPressed;
            break;
        case sf::Keyboard::Down:
            mario->isMovingDown = mario->isMoving = isPressed;
            break;
        case sf::Keyboard::Left:
            mario->isMovingLeft = mario->isMoving = isPressed;
            break;
        case sf::Keyboard::Right:
            mario->isMovingRight = mario->isMoving = isPressed;
            break;

        // TODO: Jump!!!
        case sf::Keyboard::Space:
            break;
    }
}

void Game::handleCoins() {
    auto coins = EntityManager::GetCoins();
    auto playerBounds = sf::Rect<float>(
        mario->m_position.x,
        mario->m_position.y,
        mario->m_size.x,
        mario->m_size.y
    );

    for (auto const& coin: coins) {
        auto coinGlobalBounds = coin.get()->m_sprite.getGlobalBounds();

        if (coinGlobalBounds.intersects(playerBounds)) {
            EntityManager::RemoveCoin(coin);
            score += COIN_VALUE;
        }
    }
}
