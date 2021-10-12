//! \file

#ifndef YACK_WTIME_INCLUDED
#define YACK_WTIME_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    class wtime
    {
    public:

        explicit wtime();
        virtual ~wtime() throw();

        static uint64_t  ticks();     //!< system ticks


        //! frequency to convert ticks to seconds
        const
#if defined(YACK_DARWIN) || defined(YACK_WIN)
        double
#else
        uint64_t
#endif
        freq;
    private:
        YACK_DISABLE_COPY_AND_ASSIGN(wtime);
    };

}

#endif

