//! \file

#ifndef YACK_WTIME_INCLUDED
#define YACK_WTIME_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! get wall ticks/time
    //
    //__________________________________________________________________________
    class wtime
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        explicit wtime();          //!< setup frequency
        virtual ~wtime() throw();  //!< cleanup

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        static uint64_t  ticks();                //!< system ticks
        double operator()(const uint64_t) const; //!< ticks->seconds
        void   wait(const double secs) const;    //!< empty loop over secs seconds

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
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

