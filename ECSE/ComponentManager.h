#pragma once

#include <map>
#include <typeinfo>
#include <type_traits>
#include <boost/pool/object_pool.hpp>
#include "Component.h"
#include "Pool.h"

namespace ECSE {

//! Handles allocation and deallocation for all component types.
/*!
* This class allows Components of the same type to remain more or less tightly-packed. Ownership of
* the allocated Components will always remain within the ComponentManager.
*/
class ComponentManager
{
public:
    //! Destroy the ComponentManager.
    virtual ~ComponentManager() {};

    //! Create a new Component.
    /*!
    * \return A pointer to the new Component.
    */
    template <typename ComponentType>
    ComponentType* createComponent();

    //! Destroy a Component.
    /*!
    * \param component A pointer to the Component to be destroyed (must be from this manager's pool).
    */
    template <typename ComponentType>
    void destroyComponent(ComponentType* component);

private:
    //! Get the internal pool from a generic PoolBase.
    /*!
    * \param pool The generic pool.
    * \return The internal Boost pool.
    */
    template <typename ComponentType>
    static boost::object_pool<ComponentType>& getInternalPool(PoolBase& pool);

    //! Get the collection of Components of a given type.
    /*!
    * \return A reference to the Component pool.
    */
    template <typename ComponentType>
    PoolBase& getPool();

    //! Map from component type to pools of Components.
    std::map<size_t, std::unique_ptr<PoolBase>> pools;

    //! Map from component pointer to pool.
    std::map<Component*, PoolBase*> directory;
};

//////////////////
// Implementation

template <typename ComponentType>
boost::object_pool<ComponentType>& ComponentManager::getInternalPool(PoolBase& pool)
{
    typedef Pool<ComponentType> PType;

    return static_cast<PType*>(&pool)->pool;
}

template <typename ComponentType>
ComponentType* ComponentManager::createComponent()
{
    // Get the pool of components for this type
    auto& pool = getPool<ComponentType>();
    auto& typedPool = getInternalPool<ComponentType>(pool);

    // Attempt to allocate a new component
    ComponentType* component = typedPool.construct();
    if (!component)
    {
        throw std::runtime_error("Out of memory!");
    }

    directory[component] = &pool;

    return component;
}

template <typename ComponentType>
void ComponentManager::destroyComponent(ComponentType* component)
{
    auto directoryIt = directory.find(component);
    if (directoryIt == directory.end())
    {
        throw std::runtime_error("Attempted to delete a component which was not created by this manager");
    }

    // Get the pool of components for this type
    auto pool = directoryIt->second;
    auto& typedPool = getInternalPool<ComponentType>(*pool);

    // Attempt to free the component
    assert(typedPool.is_from(component));
    typedPool.destroy(component);
}

template <typename ComponentType>
PoolBase& ComponentManager::getPool()
{
    static_assert(std::is_base_of<Component, ComponentType>::value,
                  "ComponentType must be a descendant of Component!");

    typedef Pool<ComponentType> PType;

    size_t typeHash = typeid(ComponentType).hash_code();

    // Get the base class pool pointer pool
    PoolBase* pool = pools[typeHash].get();
    if (!pool)
    {
        pools[typeHash] = std::unique_ptr<PType>(new PType);
        pool = pools[typeHash].get();
    }

    return *pool;
}

}
