#pragma once

#include <stdexcept>
#include "Component.h"
#include "World.h"

namespace ECSE
{

//! A class to which the SpecializationComponent/SpecializationSystem defers.
class Specialization
{
    friend class SpecializationSystem;

public:
    //! Destroy the Specialization.
    virtual ~Specialization() {}

    //! Called when the Specialization is ready to interact with the World and other Entities.
    virtual void init() {}

    //! Called on an update step.
    /*!
    * The Specialization should react to its current states, then prepare its next state.
    *
    * \param deltaTime The amount of elapsed time to simulate.
    */
    virtual void update(sf::Time deltaTime) {}

    //! Called on an advance step.
    /*!
    * The Specialization should update its current state to advance the simulation.
    */
    virtual void advance() {}

    //! Called on a render step.
    /*!
    * The Specialization should be drawn, interpolated between its current and next state.
    *
    * \param alpha The amount of interpolation between the two states.
    * \param renderTarget The RenderTarget to draw to.
    */
    virtual void render(float alpha, sf::RenderTarget& renderTarget) {}

protected:
    Entity* entity = nullptr;   //!< The entity to which this is attached.
    World* world = nullptr;     //!< The world in which this exists.
};


//! A component which defers to a more complex Specialization class, avoiding the need for a System.
/*!
* A SpecializationComponent is useful in cases where complex behaviour is needed from a single Entity.
* For example, a unique boss with many different states and attacks may warrant use of this component.
*
* Of course, overuse of this class defeats the purpose of using components at all.
* If you're not sure whether to use a SpecializationComponent, ask yourself:
* - Will there be multiple entities using the same Specialization simultaneously?
* - Will a deep Specialization class hierarchy be required to produce the desired behaviour?
*
* If the answer to either of these questions is "yes," consider using Component and System instead.
*/
class SpecializationComponent : public Component
{
public:
    //! Set the component's specialization. This should only be called once.
    /*!
    * \param spec The specialization to use.
    */
    inline void setSpecialization(std::unique_ptr<Specialization> spec)
    {
        if (spec == nullptr)
        {
            throw std::runtime_error("Specialization must not be null");
        }

        if (this->spec != nullptr)
        {
            throw std::runtime_error("Specialization already set");
        }

        this->spec = std::move(spec);
    }

    //! Get the component's specialization.
    /*!
    * \param spec The specialization.
    */
    inline Specialization* getSpecialization()
    {
        return spec.get();
    }

private:
    //! The class which will receive events.
    std::unique_ptr<Specialization> spec = nullptr;
};

}