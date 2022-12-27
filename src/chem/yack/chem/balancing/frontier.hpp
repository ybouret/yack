
//! \file

#ifndef YACK_CHEM_FRONTIER_INCUDED
#define YACK_CHEM_FRONTIER_INCUDED 1

#include "yack/chem/species.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! frontier = a list of species with the same frontier extent
        //
        //______________________________________________________________________
        class frontier : public sp_repo
        {

        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit frontier(const sp_fund &) throw(); //!< setup
            virtual ~frontier() throw();                //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void                  destroy() throw(); //!< release list, xi=0
            friend std::ostream & operator<<(std::ostream &, const frontier &); //!< display


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const double   xi; //!< frontier extent at which species vanish

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(frontier);
        };


    }

}

#endif
