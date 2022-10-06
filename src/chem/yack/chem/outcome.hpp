//! \file

#ifndef YACK_CHEM_OUTCOME_INCLUDED
#define YACK_CHEM_OUTCOME_INCLUDED 1

#include "yack/chem/components.hpp"

namespace yack
{
    namespace chemical
    {
       
        //______________________________________________________________________
        //
        //
        //! 1D outcome
        //
        //______________________________________________________________________
        class outcome
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            outcome() throw();                                                         //!< blocked, degenerated, 0
            outcome(const components::state,const extent::grade,const double) throw(); //!< setup
            outcome(const outcome &) throw();                                          //!< no-throw copy
            outcome & operator=(const outcome &other) throw();                         //!< assign
            ~outcome() throw();                                                        //!< cleanup

            //__________________________________________________________________
            //
            //! solve 1D, return full outcome
            //__________________________________________________________________
            static outcome study(const components       &comp,
                                 const double            K,
                                 const readable<double> &Cini,
                                 writable<double>       &Cend,
                                 rmulops                &xmul,
                                 raddops                &xadd,
                                 size_t                 *call=0);


            //__________________________________________________________________
            //
            //! return value or zero if degenerated
            //__________________________________________________________________
            double operator*() const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const components::state state; //!< state of components
            const extent::grade     grade; //!< grade of extent
            const double            value; //!< extent value


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! textual output
            friend std::ostream & operator<<(std::ostream &os, const outcome &out);

        };

    }
}

#endif
