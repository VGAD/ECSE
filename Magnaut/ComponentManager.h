#pragma once

#include <map>
#include <vector>
#include <typeinfo>
#include <type_traits>
#include <boost/pool/object_pool.hpp>
#include "Common.h"
#include "Component.h"
#include "Pool.h"

//! Handles allocation and deallocation for all component types.
class ComponentManager
{
public:
    //! Construct the ComponentManager.
    ComponentManager();

    //! Destroy the ComponentManager.
    ~ComponentManager();

    //! Create a new component.
    template <typename ComponentType>
    ComponentType* create();

    //! Destroy a component
    template <typename ComponentType>
    void destroy(ComponentType* component);

private:
    //! Get the collection of components of a given type.
    template <typename ComponentType>
    boost::object_pool<ComponentType>& getPool();

    //! Map from component type to pools of Components.
    std::map<size_t, std::unique_ptr<PoolBase>> pools;
};

//////////////////
// Implementation

template <typename ComponentType>
ComponentType* ComponentManager::create()
{
    // Get the pool of components for this type
    boost::object_pool<ComponentType>& pool = getPool<ComponentType>();

    // Attempt to allocate a new component
    ComponentType* component = pool.malloc();
    if (!component) {
        throw std::runtime_error("Out of memory!");
    }

    return component;
}

template <typename ComponentType>
void ComponentManager::destroy(ComponentType* component)
{
    // Get the pool of components for this type
    boost::object_pool<ComponentType>& pool = getPool<ComponentType>();

    // Attempt to free the component
    assert(pool.is_from(component));
    pool.free(component);
}

template <typename ComponentType>
boost::object_pool<ComponentType>& ComponentManager::getPool()
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

    // Cast to the actual class type
    PType* typedPool = static_cast<PType*>(pool);

    return typedPool->pool;
}