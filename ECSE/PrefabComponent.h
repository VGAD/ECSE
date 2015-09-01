#pragma once

#include "Component.h"

namespace ECSE {

//! A Component which stores info about the prefab from which the entity was created.
class PrefabComponent : public Component
{
    friend class PrefabManager;

public:
    //! Get the name of the prefab from which the entity was created.
    /*!
    * \return The prefab name.
    */
    inline const std::string& getPrefabName() const { return prefabName; }

private:
    std::string prefabName;     //!< The name of the prefab from which the entity was created.
};

}
