//! \file

#ifndef YACK_AQUEOUS_EQUILIBRIUM_INCLUDED
#define YACK_AQUEOUS_EQUILIBRIUM_INCLUDED 1

#include "yack/aqueous/components.hpp"

namespace yack
{
    namespace aqueous
    {

        //______________________________________________________________________
        //
        //
        //! equilibrium = components + entity + constant(time)
        //
        //______________________________________________________________________
        class equilibrium : public components, public entity
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef ark_ptr<string,equilibrium> ptr; //!< shared pointer

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual equilibrium *clone() const = 0; //!< for virtual copy
            double               K(double);         //!< check getK(t) > 0

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~equilibrium() noexcept; //!< cleanup

        protected:
            //! setup empty
            template <typename NAME> inline
            explicit equilibrium(NAME        &uid,
                                 const size_t idx) :
            components(),
            entity(uid,idx)
            {
            }

            //! hard copy
            equilibrium(const equilibrium &);

        private:
            YACK_DISABLE_ASSIGN(equilibrium);
            virtual double getK(double) = 0;
        };

        //______________________________________________________________________
        //
        //
        //! built-in constant equilibrium
        //
        //______________________________________________________________________
        class const_equilibrium : public equilibrium
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            template <typename NAME> inline
            explicit const_equilibrium(NAME &uid, const double K0, const size_t idx) :
            equilibrium(uid,idx),
            K_(K0)
            {
            }
            const_equilibrium(const const_equilibrium &); //!< copy
            virtual ~const_equilibrium() noexcept;        //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual equilibrium *clone() const; //!< new const_equilibrium(*this)

        private:
            YACK_DISABLE_ASSIGN(const_equilibrium);
            const double K_;
            virtual double getK(double) noexcept;
        };

    }

}

#endif