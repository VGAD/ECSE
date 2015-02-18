#pragma once

#include <set>
#include "System.h"

//! A System which maintains a set of Entities as its internal structure.
namespace ECSE
{

class SetSystem :
    public System
{
public:
    //! Construct the SetSystem.
    SetSystem(World* world) : System(world) {}

    //! Destroy the SetSystem.
    virtual ~SetSystem() {}

    //! Return whether an Entity is already in the System.
    /*!
    * \param e The Entity to check.
    * \return Whether the Entity is already in the System's set.
    */
    inline bool hasEntity(const Entity& e) const
    {
        return entities.find(const_cast<Entity*>(&e)) != entities.end();
    }

    //! Get the Entities contained in the SetSystem.
    inline const std::set<Entity*>& getEntities() const
    {
        return entities;
    }

protected:
    //! Add an Entity to the internal Entity set.
    /*!
    * \param e The Entity to add.
    */
    inline void internalAddEntity(Entity& e)
    {
        LOG(DEBUG) << "Entity #" << e.getID() << " added to SetSystem";

        entities.insert(&e);
    }

    //! Remove an Entity from the internal Entity set.
    /*!
    * \param e The Entity to remove.
    */
    inline void internalRemoveEntity(Entity& e)
    {
        LOG(DEBUG) << "Entity #" << e.getID() << " removed from SetSystem";

        entities.erase(&e);
    }

private:
    std::set<Entity*> entities;   //!< The set of Entities operated on by this SetSystem.
};


}
