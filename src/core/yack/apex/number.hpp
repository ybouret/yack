
//! \file

#ifndef YACK_APEX_NUMBER_INCLUDED
#define YACK_APEX_NUMBER_INCLUDED 1

#include "yack/object.hpp"

namespace yack
{

    namespace apex
    {

#define YACK_APEX_TRACKING 1

        //______________________________________________________________________
        //
        //
        //! base class for apex
        //
        //______________________________________________________________________
        class number : public object
        {
        public:
            static uint64_t add_ticks;
            static size_t   add_count;
            static uint64_t sub_ticks;
            static size_t   sub_count;
            static uint64_t lmul_ticks;
            static size_t   lmul_count;

            static void     reset_tracking() throw();
            static double   rate(const uint64_t ell, const size_t num);
            static double   add_rate();
            static double   sub_rate();
            static double   lmul_rate();


            virtual ~number() throw(); //!< cleanup

        protected:
            explicit number() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(number);
        };
    }

}

#endif
