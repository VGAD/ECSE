#pragma once

#include "SetSystem.h"

namespace ECSE
{
 
//! A system which interacts with SpecializationComponents and their corresponding Specializations.
class SpecializationSystem : public SetSystem
{
public:
    //! Construct the SpecializationSystem.
    explicit SpecializationSystem(World* world) : SetSystem(world) {}

    //! Called on an update step.
    /*!
    * \param deltaTime Time elapsed since the last update.
    */
    void update(sf::Time deltaTime) override;

    //! Called on an advance step.
    void advance() override;

    //! Called on a render step.
    /*!
    * \param alpha The amount of interpolation between the two states.
    * \param renderTarget The RenderTarget to draw to.
    */
    void render(float alpha, sf::RenderTarget& renderTarget) override;

    //! Check whether an Entity meets the requirements to be added to this System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity matches this System's requirements.
    */
    bool checkRequirements(const Entity& e) const override;

protected:
    //! Add an Entity to the internal Entity set.
    /*!
    * \param e The Entity to add.
    */
    void internalAddEntity(Entity& e) override;
};

}
