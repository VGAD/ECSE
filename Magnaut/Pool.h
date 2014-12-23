#pragma once

//! Base class for object pools. Useful for maintaining containers of pointers to various Pool types.
/*!
  This class should probably only be used if you want to have a bunch of different pools of different
  types and need a base class pointer. Otherwise, you're just wasting time and space on this.
*/
struct PoolBase
{
    virtual ~PoolBase() { }
};

//! Specialized collection for each class type.
/*!
  \see PoolBase
*/
template <typename T>
struct Pool : PoolBase
{
    //! The actual object pool.
    boost::object_pool<T> pool;
};