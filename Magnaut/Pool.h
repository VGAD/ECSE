#pragma once

//! Base class for object pools. Useful for maintaining containers of pointers to various Pool types.
struct PoolBase
{
    virtual ~PoolBase() { }
};

//! Specialized collection for each class type
template <typename T>
struct Pool : PoolBase
{
    //! The actual object pool.
    boost::object_pool<T> pool;
};