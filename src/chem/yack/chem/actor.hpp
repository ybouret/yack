//! \file

#ifndef YACK_CHEM_ACTOR_INCLUDED
#define YACK_CHEM_ACTOR_INCLUDED 1

#include "yack/chem/component.hpp"
#include "yack/type/authority.hpp"

namespace yack
{

    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! actor in an equilibrium
        //
        //______________________________________________________________________
        class actor : public object, public authority<const species>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit actor(const component &) throw(); //!< setup
            virtual ~actor() throw();                  //!< cleanup


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            double             extent(const readable<double> &C)           const throw(); //!< (**this)(C)/nu
            double             action(const readable<double> &C)           const throw(); //!< (**this)(C)^nu
            double             derivs(double K, const readable<double> &C) const throw(); //!< K * nu * (**this)(C)^nu1
            writable<double> & ldz(writable<double> &C)                    const throw(); //!< (**this)(C) = 0

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t nu;   //!< |nu| > 0
            const size_t nu1;  //!< nu-1
            actor       *next; //!< for list
            actor       *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(actor);
        };

     
        

    }

}

#endif

