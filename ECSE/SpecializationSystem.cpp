#include "SpecializationSystem.h"
#include "SpecializationComponent.h"

namespace ECSE {

void SpecializationSystem::update(sf::Time deltaTime)
{
    for (Entity* e : getEntities())
    {
        auto specComponent = e->getComponent<SpecializationComponent>();
        if (!specComponent->enabled) continue;

        auto spec = specComponent->getSpecialization();
        assert(spec != nullptr);

        spec->update(deltaTime);
    }
}

void SpecializationSystem::advance()
{
    SetSystem::advance();

    for (Entity* e : getEntities())
    {
        auto specComponent = e->getComponent<SpecializationComponent>();
        if (!specComponent->enabled) continue;

        auto spec = specComponent->getSpecialization();
        assert(spec != nullptr);

        spec->advance();
    }
}

void SpecializationSystem::render(float alpha, sf::RenderTarget& renderTarget)
{
    for (Entity* e : getEntities())
    {
        auto specComponent = e->getComponent<SpecializationComponent>();
        if (!specComponent->enabled) continue;

        auto spec = specComponent->getSpecialization();
        assert(spec != nullptr);

        spec->render(alpha, renderTarget);
    }
}

bool SpecializationSystem::checkRequirements(const Entity& e) const
{
    if (!e.getComponent<SpecializationComponent>()) return false;

    return true;
}

void SpecializationSystem::internalAddEntity(Entity& e)
{
    SetSystem::internalAddEntity(e);

    auto component = e.getComponent<SpecializationComponent>();
    auto spec = component->getSpecialization();

    spec->entity = &e;
    spec->world = world;

    spec->init();
}

}