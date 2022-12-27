
//! \file

#ifndef YACK_CHEM_LIMITING_INCUDED
#define YACK_CHEM_LIMITING_INCUDED 1

#include "yack/chem/balancing/frontier.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! lower limiting, with matching vanishing species
        //
        //______________________________________________________________________
        class limiting : public frontier
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit limiting(const sp_fund &) throw(); //!< setup
            virtual ~limiting() throw();                //!< cleanup


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! upgrade current status
            /**
             - initialize if first species
             - keep smallest or update list of vanishing species
             */

            void upgrade(const double x, const species &s);
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(limiting);
        };


    }

}

#endif
