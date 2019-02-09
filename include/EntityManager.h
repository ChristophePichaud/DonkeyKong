#pragma once

#include <list>
#include "Entity.h"

class EntityManager {
    public:
        EntityManager() = default;

        ~EntityManager() = default;

    public:
        static std::list<std::shared_ptr<Entity>> m_Entities;
        static std::list<std::shared_ptr<Entity>> GetCoins();
        static void RemoveCoin(std::shared_ptr<Entity> coin);
};

