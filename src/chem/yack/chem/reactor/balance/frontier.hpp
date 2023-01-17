//! \file

#ifndef YACK_CHEMICAL_FRONTIER_INCLUDED
#define YACK_CHEMICAL_FRONTIER_INCLUDED 1

#include "yack/chem/actors.hpp"
#include "yack/data/dinky/coop-repo.hpp"
#include "yack/system/arch.h"

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
        class frontier :  public sp_para
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            frontier(const sp_fund &) throw();      //!< setup
            virtual ~frontier()       throw();      //!< cleanup
            frontier(const frontier &);             //!< copy
            frontier & operator=(const frontier &); //!< assign

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            void             free()                            throw();              //!< xi=0, clear()
            void             vanish(writable<double> &C) const throw();              //!< set all registered species to exactly zero
            bool             adjust(const readable<double> &C, const actors &A);     //!< adjustement in case of negative value(s) for actors
            YACK_PROTO_OSTREAM(frontier);                                            //!< display
            void             operator()(const double xx, const species &s);          //!< update xx>=0 for limiting

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            double   xi; //!< at xi, species will vanish
            

        private:
#if 64 == YACK_NATIVE_BITS
            const double unused;
#endif
        };
    }

}

#endif
