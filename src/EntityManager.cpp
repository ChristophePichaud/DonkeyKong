#include "pch.h"
#include "EntityManager.h"


std::vector<std::shared_ptr<Entity>> EntityManager::m_Entities;

std::shared_ptr<Entity> EntityManager::GetPlayer() {
    for (std::shared_ptr<Entity> entity : EntityManager::m_Entities) {
        if (!entity->m_enabled) {
            continue;
        }

        if (entity->m_type == EntityType::player) {
            return entity;
        }
    }

    return nullptr;
}
