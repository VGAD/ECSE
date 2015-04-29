#pragma once

//! A class which holds data about an Entity.
namespace ECSE
{

/*!
* Components may include convenience functions for operating on their data, but should
* generally not be aware of other Components. Any function that needs to deal with more
* than one Component should probably go in a System.
*/
class Component
{
public:
    friend class Entity;

    //! Whether Systems should update this.
    /*!
    * Effectively, the System should behave as if this was not attached to its Entity.
    */
    bool enabled = true;

protected:
    //! Called when this is attached to an Entity.
    /*!
    * This is a good place to check requirements and/or notify other Components.
    * \param e The Entity to which this was attached.
    */
    virtual void attached(Entity* e) {};
};


}
