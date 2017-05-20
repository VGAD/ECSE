#pragma once

#include "SetSystem.h"
#include "TransformSystem.h"
#include "Engine.h"
#include "ColliderComponent.h"
#include "CollisionSystem.h"

namespace ECSE
{

//! A System that draws collision detection debug information.
class CollisionDebugSystem :
    public SetSystem
{
public:
    ////////////
    // Functions

    //! Construct the CollisionDebugSystem.
    explicit CollisionDebugSystem(World* world);

    //! Called on an advance step.
    /*!
    * First, updates the internal list of Entities.
    * Next, each Entity should update its current state to advance the simulation.
    *
    * If you override this, make sure to call System::advance() before your own code.
    */
    void advance() override;

    //! Called on a render step.
    /*!
    * Each Entity should be drawn, interpolated between its current and next state.
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
    bool checkRequirements(const Entity& e) const override;

    //! Called when all Systems have been added to the world.
    /*!
    * Override this to get references to other Systems.
    */
    virtual void added() override;


    ///////
    // Data

    //! Whether logging should be verbose.
    /*!
    * This may cause slowdown with a lot of collisions happening.
    */
    bool verbose = false;

    //! Whether to draw movement trails.
    bool drawTrails = true;

    //! Whether to draw collisions.
    bool drawCollisions = true;

protected:
    //! Add an Entity to the internal Entity set.
    /*!
    * \param e The Entity to add.
    */
    void internalAddEntity(Entity& e) override;

private:
    ////////////
    // Functions
    
    //! Draw the sweep of an entity's circle collider (if it has one).
    /*!
    * \param e The Entity.
    */
    void drawCircleSweep(const Entity& e);

    //! Draw the sweep of an entity's line collider (if it has one).
    /*!
    * \param e The Entity.
    */
    void drawLineSweep(const Entity& e);

    //! Draw a collision.
    /*!
    * \param The collision to draw.
    */
    void drawCollision(const Collision& collision);

    //! Draw a line.
    /*!
    * \param start The start point of the line.
    * \param end The end point of the line.
    * \param color The line color.
    */
    void drawLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color);

    //! Callback function for collisions.
    /*!
    * Adds the collision to the buffer.
    * \param collision The collision to add.
    * \return An empty set.
    */
    ColliderComponent::ChangeSet collisionCallback(const Collision& collision);


    ///////
    // Data
    
    //! The RenderTarget to which this will draw.
    sf::RenderTarget& renderTarget;

    //! The circle shape reused in drawing.
    sf::CircleShape circleShape;

    //! The CollisionSystem of this System's World.
    CollisionSystem* collisionSystem;

    //! Buffer of collisions that occurred this frame
    std::vector<Collision> collisionBuffer;
};

}
