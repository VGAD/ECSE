#include "RenderSystem.h"
#include "SpriteComponent.h"
#include "DepthComponent.h"
#include "TransformComponent.h"
#include "TransformSystem.h"
#include "World.h"
#include "Engine.h"

bool RenderSystem::hasEntity(const Entity& e) const
{
    return layers.find(const_cast<Entity*>(&e)) != layers.end();
}

void RenderSystem::update(sf::Time deltaTime)
{
    for (auto& pair : layers)
    {
        Spritemap& sprite = pair.first->getComponent<SpriteComponent>()->sprite;
        sprite.update(deltaTime);
    }
}

void RenderSystem::render(float alpha)
{
    // First, make sure entities are sorted
    sortLayers();

    sf::RenderTarget& target = *world->getEngine()->getRenderTarget();

    // Iterate layers, which are already sorted from highest to lowest depth
    for (auto& pair : entities)
    {
        for (auto& entity : pair.second)
        {
            Spritemap& sprite = entity->getComponent<SpriteComponent>()->sprite;

            // Update sprite transform
            TransformSystem* ts = world->getSystem<TransformSystem>();
            sprite.setPosition(ts->interpolatePosition(*entity, alpha));
            sprite.setRotation(radToDeg(ts->interpolateAngle(*entity, alpha)));

            // Render it
            target.draw(sprite);
        }
    }
}

bool RenderSystem::checkRequirements(const Entity& e) const
{
    if (!e.getComponent<TransformComponent>()) return false;
    if (!e.getComponent<SpriteComponent>()) return false;
    if (!e.getComponent<DepthComponent>()) return false;

    return true;
}

void RenderSystem::internalAddEntity(Entity& e)
{
    LOG(DEBUG) << "Entity #" << e.getID() << " added to RenderSystem";

    int layer = e.getComponent<DepthComponent>()->depth;

    layers[&e] = layer;
    entities[layer].insert(&e);
}

void RenderSystem::internalRemoveEntity(Entity& e)
{
    LOG(DEBUG) << "Entity #" << e.getID() << " removed from RenderSystem";

    int layer = e.getComponent<DepthComponent>()->depth;

    layers.erase(&e);

    auto& entitySet = entities[layer];
    entitySet.erase(&e);

    // Remove empty layer
    if (entitySet.empty())
    {
        entities.erase(layer);
    }
}

void RenderSystem::sortLayers()
{
    for (auto& pair : layers)
    {
        int layer = pair.first->getComponent<DepthComponent>()->depth;

        // Change layers
        if (pair.second != layer)
        {
            entities[pair.second].erase(pair.first);

            pair.second = layer;
            entities[layer].insert(pair.first);
        }
    }
}