//! \file

#ifndef YACK_CHEM_LIMITS_INCLUDED
#define YACK_CHEM_LIMITS_INCLUDED 1

#include "yack/chem/eq/limiting.hpp"

namespace yack
{

    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! limits for the extent of one equilibrium
        //
        //______________________________________________________________________
        class limits
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            limits(const limiting *r, const limiting *p) throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! display
            friend std::ostream & operator<<(std::ostream &, const limits &);

            double crop(const double xi) const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const limiting  * const reac; //!< limiting reactant/NULL
            const limiting  * const prod; //!< limiting product/NULL
            const limitation        type; //!< classification


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(limits);
            limitation make() const throw();
            ~limits() throw();
        };

    }

}

#endif

