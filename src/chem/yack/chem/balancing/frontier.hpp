
//! \file

#ifndef YACK_CHEM_FRONTIER_INCUDED
#define YACK_CHEM_FRONTIER_INCUDED 1

#include "yack/chem/sp/repo.hpp"
#include "yack/chem/actor.hpp"

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
            void                  reverse() throw(); //!< change current xi sign
            friend std::ostream & operator<<(std::ostream &, const frontier &); //!< display

            //! set border species to 0
            void ldz(writable<double> &C) const throw();
            
            //! move all concentrations with xi from first actor 
            void mov(writable<double> &C, const actor *curr) const throw();


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
