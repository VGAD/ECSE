#pragma once

#include "System.h"

//! A system which renders entities to the screen.
namespace ECSE
{

class RenderSystem :
    public System
{
public:
    //! Construct the RenderSystem.
    RenderSystem(World* world) : System(world) {}

    //! Return whether an Entity is already in the RenderSystem.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity is already in the RenderSystem's internal structure.
    */
    bool hasEntity(const Entity& e) const;

    //! Called on an update step.
    /*!
    * Sprites' frames are updated.
    */
    void update(sf::Time deltaTime);

    //! Called on a render step.
    /*!
    * Layers are sorted as necessary, then each Entity is drawn.
    *
    * \param alpha The amount of interpolation between the two states.
    */
    void render(float alpha);

    //! Check whether an Entity meets the requirements to be added to this System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity matches this System's requirements.
    */
    bool checkRequirements(const Entity& e) const;

private:
    ////////////
    // Functions

    //! Add an Entity to the internal System structure.
    /*!
    * \param e The Entity to add.
    */
   void internalAddEntity(Entity& e);

    //! Remove an Entity from the internal System structure.
    /*!
    * \param e The Entity to remove.
    */
    void internalRemoveEntity(Entity& e);

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
