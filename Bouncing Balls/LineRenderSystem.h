#pragma once

#include "ECSE/SetSystem.h"
#include "ECSE/TransformSystem.h"
#include "ECSE/Engine.h"
#include "ECSE/ColliderComponent.h"
#include "ECSE/CollisionSystem.h"

namespace BouncingBalls
{

//! A System that draws object trajectories.
class LineRenderSystem :
    public ECSE::SetSystem
{
public:
    ////////////
    // Functions

    //! Construct the LineRenderSystem.
    explicit LineRenderSystem(ECSE::World* world);

    //! Called when this is added to the world.
    void added() override;

    //! Render lines.
    void render(float alpha, sf::RenderTarget& renderTarget) override;

    //! Check whether an Entity meets the requirements to be added to this System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity matches this System's requirements.
    */
    bool checkRequirements(const ECSE::Entity& e) const override;

private:
    ////////////
    // Functions

    //! Draw an entity's line collider.
    /*!
    * \param e The Entity.
    */
    void drawLineEntity(const ECSE::Entity& e);

    //! Draw a line.
    /*!
    * \param start The start point of the line.
    * \param end The end point of the line.
    * \param color The line color.
    */
    void drawLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color);


    ///////
    // Data
    
    //! The RenderTarget to which this will draw.
    sf::RenderTarget& renderTarget;

    //! The CollisionSystem of this System's World.
    ECSE::CollisionSystem* collisionSystem;
};

}
