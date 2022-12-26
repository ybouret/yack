
//! \file

#ifndef YACK_CHEMICAL_BALANCING_INCLUDED
#define YACK_CHEMICAL_BALANCING_INCLUDED 1

#include "yack/chem/nexus.hpp"
#include "yack/type/authority.hpp"
#include "yack/chem/limiting.hpp"
#include "yack/chem/boundary.hpp"

namespace yack
{

    namespace chemical
    {
        //______________________________________________________________________
        //
        //
        //! balancing algorithm
        //
        //______________________________________________________________________
        class balancing : public authority<const nexus>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit balancing(const nexus &, const xmlog &); //!< setup
            virtual ~balancing() throw();                     //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool operator()(writable<double> &C0); //!< balance C0
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const xmlog &xml; //!< helper
            sp_fund      io;  //!< shared
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(balancing);
            bool balance(writable<double> &C0,
                         const cluster    &cc);
            
        };


    }

}

#endif

