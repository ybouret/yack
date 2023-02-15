//! \file

#ifndef YACK_CHEM_AFTERMATH_INCLUDED
#define YACK_CHEM_AFTERMATH_INCLUDED 1

#include "yack/chem/components.hpp"

namespace yack
{
    namespace chemical
    {
        //______________________________________________________________________
        //
        //! one dimensional solver
        //
        //______________________________________________________________________
        class aftermath
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            aftermath()                                      throw(); //!< blocked/zero
            ~aftermath()                                     throw(); //!< cleanup
            aftermath(const components::state, const double) throw(); //!< setup
            aftermath(const aftermath &)                     throw(); //!< copy
            aftermath & operator=( const aftermath &)        throw(); //!< assign
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! compute Cend from Cini and return result
            static aftermath  guess(const components       &comp,
                                    const double            K_eq,
                                    const readable<double> &Cini,
                                    writable<double>       &Cend,
                                    raddops                &xadd,
                                    rmulops                &xmul);
            
           
            //! display
            friend std::ostream & operator<<(std::ostream &, const aftermath &);
            
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const components::state state; //!< state of components
            const double            value; //!< extent value
            
        };
    }
    
}

#endif
