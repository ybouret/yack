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
            outcome(const components::state,const extent::grade,const double) throw(); //!< setup
            outcome(const outcome &) throw();                                          //!< no-throw copy
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
            // members
            //__________________________________________________________________
            const components::state state; //!< state of components
            const extent::grade     grade; //!< grade of extent
            const double            value; //!< extent value


        private:
            YACK_DISABLE_ASSIGN(outcome);
        };

    }
}

#endif
