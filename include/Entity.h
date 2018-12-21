#pragma once


enum EntityType
{
    player,
    block,
    scale,
    unknown
};

class Entity
{
public:
    Entity() = default;

    ~Entity() = default;

public:
    sf::Sprite m_sprite;
    sf::Vector2u m_size;
    sf::Vector2f m_position;
    EntityType m_type = EntityType::unknown;
    bool m_enabled = true;

    // Enemy only
    bool m_bLeftToRight = true;
    int m_times = 0;
};

