#pragma once

#include <set>
#include "System.h"

//! A System which maintains a set of Entities as its internal structure.
class SetSystem :
    public System
{
public:
    //! Construct the SetSystem.
    SetSystem(World* world) : System(world) {}

    //! Destroy the SetSystem.
    virtual ~SetSystem();

    //! Return whether an Entity is already in the System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity is already in the System's set.
    */
    virtual bool hasEntity(const Entity& e) const;

    //! Get the Entities contained in the SetSystem.
    const std::set<Entity*>& getEntities() const;

protected:
    //! Add an Entity to the internal Entity set.
    /*!
    * \param e The Entity to add.
    */
    virtual void internalAddEntity(Entity& e);

    //! Remove an Entity from the internal Entity set.
    /*!
    * \param e The Entity to remove.
    */
    virtual void internalRemoveEntity(Entity& e);

private:
    std::set<Entity*> entities;   //!< The set of Entities operated on by this SetSystem.
};
