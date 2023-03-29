


//! \file

#ifndef YACK_AQUEOUS_REACTOR_INCLUDED
#define YACK_AQUEOUS_REACTOR_INCLUDED 1

#include "yack/aqueous/realm/domain.hpp"
#include "yack/aqueous/eqs/xlimits.hpp"

namespace yack
{
    namespace aqueous
    {

        class reactor
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
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
        };
    }

}

#endif
