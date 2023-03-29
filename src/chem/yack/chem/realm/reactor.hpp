//! \file

#ifndef YACK_CHEMICAL_REACTOR_INCLUDED
#define YACK_CHEMICAL_REACTOR_INCLUDED 1

#include "yack/chem/realm/domain.hpp"
#include "yack/chem/eqs/xlimits.hpp"

namespace yack
{
    namespace chemical
    {

        class reactor : public spot_object
        {
        public:
            explicit reactor(const domain &);
            virtual ~reactor() noexcept;

            void operator()(writable<double>      &C0,
                            const readable<double> &K);

            const domain      &dom;     //!< persitent domain
            xlimits            xlim;
            cameo::add<double> xadd;
            cameo::mul<double> xmul;
            cxx_array<double>  Corg;    //!< [M] compact concentration
            cxx_array<bool>    blocked; //!< [L]
            cxx_array<bool>    running; //!< [L]
            cxx_array<double>  Xi;      //!< [L] compact Xi
            matrix<double>     Cs;      //!< [LxM]

            reactor *next;
            reactor *prev;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
        };
    }

}

#endif
