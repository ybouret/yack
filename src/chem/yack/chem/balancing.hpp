
//! \file

#ifndef YACK_CHEMICAL_BALANCING_INCLUDED
#define YACK_CHEMICAL_BALANCING_INCLUDED 1

#include "yack/chem/nexus.hpp"
#include "yack/type/authority.hpp"


namespace yack
{

    namespace chemical
    {


        class balancing : public authority<const nexus>
        {
        public:
            explicit balancing(const nexus &, const xmlog &);
            virtual ~balancing() throw();


            bool operator()(writable<double> &C0);

            const xmlog &xml;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(balancing);
            bool balance(writable<double> &C0,
                         const cluster    &cc);
        };


    }

}

#endif

