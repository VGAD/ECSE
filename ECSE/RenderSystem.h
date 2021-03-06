#pragma once

#include "System.h"
#include "TransformSystem.h"
#include <map>

namespace ECSE
{
class Spritemap;

//! A system which renders entities to the screen.
/*!
* Note that in order for entities to be rendered, they must have:
* - SpriteComponent
* - TransformComponent
* - DepthComponent
*/
class RenderSystem :
    public System
{
public:
    //! Construct the RenderSystem.
    explicit RenderSystem(World* world) : System(world) {}

    //! Return whether an Entity is already in the RenderSystem.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity is already in the RenderSystem's internal structure.
    */
    bool hasEntity(const Entity& e) const override;

    //! Called when all Systems have been added to the world.
    void added() override;

    //! Called on an update step.
    /*!
    * Sprites' frames are updated.
    */
    virtual void update(sf::Time deltaTime) override;

    //! Called on a render step.
    /*!
    * Layers are sorted as necessary, then each Entity is drawn.
    *
    * \param alpha The amount of interpolation between the two states.
    * \param renderTarget The RenderTarget to draw to.
    */
    virtual void render(float alpha, sf::RenderTarget& renderTarget) override;

    //! Check whether an Entity meets the requirements to be added to this System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity matches this System's requirements.
    */
    virtual bool checkRequirements(const Entity& e) const override;

protected:
    //! Update the position of an entity's sprite.
    /*!
    * This is called before renderEntity.
    *
    * \param alpha The amount of interpolation between the current and next states.
    * \param entity The entity to render.
    */
    virtual void updateSpritePos(float alpha, Entity& entity);
    
    //! Render an entity to the screen.
    /*!
    * This can be overridden to add effects.
    *
    * \param alpha The amount of interpolation between the current and next states.
    * \param renderTarget The RenderTarget to draw to.
    * \param entity The entity to render.
    */
    virtual void renderEntity(float alpha, sf::RenderTarget& renderTarget, Entity& entity);

    //! The World's TransformSystem.
    TransformSystem* ts;

protected:
    ////////////
    // Functions

    //! Add an Entity to the internal System structure.
    /*!
    * \param e The Entity to add.
    */
    void internalAddEntity(Entity& e) override;

    //! Remove an Entity from the internal System structure.
    /*!
    * \param e The Entity to remove.
    */
    void internalRemoveEntity(Entity& e) override;

    //! Re-sort the Entities into the correct layers.
    void sortLayers();

    ///////
    // Data

    //! Map from layer index to set of Entities in the layer, sorted from highest to lowest layer.
    std::map<int, std::set<Entity*>, std::greater<int>> entities;

    //! Map from Entity to layer index.
    std::map<Entity*, int> layers;
};

}
