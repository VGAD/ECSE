#pragma once

#include <stdexcept>
#include "Component.h"
#include "Specialization.h"

namespace ECSE
{

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
    * \return The specialization.
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