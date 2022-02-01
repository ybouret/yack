//! \file

#ifndef YACK_CHEM_PLEXUS_INCLUDED
#define YACK_CHEM_PLEXUS_INCLUDED 1

#include "yack/chem/equilibria.hpp"
#include "yack/chem/library.hpp"
#include "yack/sequence/arrays.hpp"
#include "yack/container/matrix.hpp"
#include "yack/apex.hpp"

namespace yack
{
    namespace chemical
    {
        
        class plexus
        {
        public:
            typedef arrays_of<double>    tableaux;
            typedef tableaux::array_type array_type;

            const library    &lib;
            const equilibria &eqs;
            
            virtual ~plexus() throw();
            explicit plexus(library &, equilibria &);

            const size_t N;
            const size_t M;
        private:
            tableaux ntab;
            tableaux mtab;
        public:
            array_type       &K;
            const matrix<int> Nu;
            const matrix<int> NuT;

            void computeK(const double t);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(plexus);
            const lockable::scope lib_lock;
        };
        
    }
}

#endif

