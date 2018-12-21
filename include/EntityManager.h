#pragma once

#include "Entity.h"

class EntityManager
{
public:
    EntityManager() = default;

    ~EntityManager() = default;

public:
    static std::vector<std::shared_ptr<Entity>> m_Entities;

    static std::shared_ptr<Entity> GetPlayer();
};

