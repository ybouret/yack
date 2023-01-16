//! \file

#ifndef YACK_CHEMICAL_FRONTIER_INCLUDED
#define YACK_CHEMICAL_FRONTIER_INCLUDED 1

#include "yack/chem/actors.hpp"
#include "yack/data/dinky/coop-repo.hpp"

namespace yack {

    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        // types for frontier sqq
        //
        //______________________________________________________________________
        typedef coop_repo<const species> sp_para; //!< shared list of species
        typedef sp_para::zpool_type      sp_bank; //!< alias
        typedef sp_para::proxy_type      sp_fund; //!< alias

        //______________________________________________________________________
        //
        //
        //! list of vanishing species at given xi
        //
        //______________________________________________________________________
        class frontier : public sp_para
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit frontier(const sp_fund &) throw(); //!< setup
            virtual ~frontier()                throw(); //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________


            void remove() throw();                                   //!< xi=0, clear()
            bool adjust(const readable<double> &C, const actors &A); //!< adjustement in case of negative value(s) for actors
            void vanish(writable<double> &C) const throw();          //!< set all registered species to exactly zero
            YACK_PROTO_OSTREAM(frontier);                            //!< display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            double   xi; //!< at xi, species will vanish
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(frontier);
        };
    }

}

#endif
