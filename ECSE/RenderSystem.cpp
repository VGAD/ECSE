#include "RenderSystem.h"
#include "SpriteComponent.h"
#include "DepthComponent.h"
#include "TransformComponent.h"
#include "TransformSystem.h"
#include "World.h"

namespace ECSE
{

bool RenderSystem::hasEntity(const Entity& e) const
{
    return layers.find(const_cast<Entity*>(&e)) != layers.end();
}

void RenderSystem::added()
{
    ts = world->getSystem<TransformSystem>();

    if (ts == nullptr)
    {
        throw std::runtime_error("RenderSystem requires a TransformSystem");
    }

    if (world->getSystemPosition<TransformSystem>() < world->getSystemPosition<RenderSystem>())
    {
        throw std::runtime_error("RenderSystem should be added before TransformSystem");
    }
}

void RenderSystem::update(sf::Time deltaTime)
{
    for (auto& pair : layers)
    {
        auto spriteComponent = pair.first->getComponent<SpriteComponent>();
        if (!spriteComponent->enabled) continue;

        auto& sprite = spriteComponent->sprite;
        sprite.update(deltaTime);
    }
}

void RenderSystem::render(float alpha, sf::RenderTarget& renderTarget)
{
    // First, make sure entities are sorted
    sortLayers();

    // Iterate layers, which are already sorted from highest to lowest depth
    for (auto& pair : entities)
    {
        for (auto& entity : pair.second)
        {
            updateSpritePos(alpha, *entity);
            renderEntity(alpha, renderTarget, *entity);
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

void RenderSystem::updateSpritePos(float alpha, Entity& entity)
{
    SpriteComponent& sc = *entity.getComponent<SpriteComponent>();
    if (!sc.enabled) return;

    Spritemap& sprite = sc.sprite;

    // Update sprite transform
    sprite.setPosition(ts->getInterpGlobalPosition(entity, alpha));
    sprite.setRotation(radToDeg(ts->getInterpGlobalAngle(entity, alpha)));
}

void RenderSystem::renderEntity(float alpha, sf::RenderTarget& renderTarget, Entity& entity)
{
    SpriteComponent& sc = *entity.getComponent<SpriteComponent>();
    if (!sc.enabled) return;

    // Render it
    renderTarget.draw(sc.sprite);
}

void RenderSystem::internalAddEntity(Entity& e)
{
    VLOG(2) << "Entity #" << e.getID() << " added to RenderSystem";

    int layer = e.getComponent<DepthComponent>()->depth;

    layers[&e] = layer;
    entities[layer].insert(&e);
}

void RenderSystem::internalRemoveEntity(Entity& e)
{
    VLOG(2) << "Entity #" << e.getID() << " removed from RenderSystem";

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

}
