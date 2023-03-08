//! \file

#ifndef YACK_AQUEOUS_EQUILIBRIUM_INCLUDED
#define YACK_AQUEOUS_EQUILIBRIUM_INCLUDED 1

#include "yack/aqueous/components.hpp"

namespace yack
{
    namespace aqueous
    {


        class equilibrium : public components, public entity
        {
        public:
            typedef ark_ptr<string,equilibrium> ptr;

            virtual equilibrium *clone() const = 0;
            double               K(double);

            //  C++
            virtual ~equilibrium() noexcept;
        protected:
            template <typename NAME> inline
            explicit equilibrium(NAME        &uid,
                                 const size_t idx) :
            components(),
            entity(uid,idx)
            {
            }

            equilibrium(const equilibrium &);


        private:
            YACK_DISABLE_ASSIGN(equilibrium);
            virtual double getK(double) = 0;
        };

        class const_equilibrium : public equilibrium
        {
        public:
            template <typename NAME> inline
            explicit const_equilibrium(NAME &uid, const double K0, const size_t idx) :
            equilibrium(uid,idx),
            K_(K0)
            {
            }
            const_equilibrium(const const_equilibrium &);
            virtual ~const_equilibrium() noexcept;

            virtual equilibrium *clone() const;

        private:
            YACK_DISABLE_ASSIGN(const_equilibrium);
            const double K_;
            virtual double getK(double) noexcept;
        };

    }

}

#endif
