
//! \file

#ifndef YACK_APEX_NUMBER_INCLUDED
#define YACK_APEX_NUMBER_INCLUDED 1

#include "yack/object.hpp"
#include "yack/ios/serializable.hpp"
#include "yack/signs.hpp"

namespace yack
{

    namespace apex
    {

#define YACK_APEX_TRACKING 1 //!< activate tracking

        //______________________________________________________________________
        //
        //
        //! base class for apex
        //
        //______________________________________________________________________
        class number : public object, public ios::serializable
        {
        public:
            static uint64_t add_ticks;  //!< cumulative ticks
            static size_t   add_count;  //!< cumulative count
            static uint64_t sub_ticks;  //!< cumulative ticks
            static size_t   sub_count;  //!< cumulative count
            static uint64_t lmul_ticks; //!< cumulative ticks
            static size_t   lmul_count; //!< cumulative count
            static uint64_t fmul_ticks; //!< cumulative ticks
            static size_t   fmul_count; //!< cumulative count
            static uint64_t fsqr_ticks; //!< cumulative ticks
            static size_t   fsqr_count; //!< cumulative count
            static uint64_t div_ticks;  //!< cumulative ticks
            static size_t   div_count;  //!< cumulative count
            static uint64_t mod_ticks;  //!< cumulative ticks
            static size_t   mod_count;  //!< cumulative count

            static void     reset_tracking() throw();                   //!< all ticks/count
            static double   rate(const uint64_t ell, const size_t num); //!< in Mops
            static double   add_rate();                                 //!< in Mops
            static double   sub_rate();                                 //!< in Mops
            static double   lmul_rate();                                //!< in Mops
            static double   fmul_rate();                                //!< in Mops
            static double   fsqr_rate();                                //!< in Mops
            static double   div_rate();                                 //!< in Mops
            static double   mod_rate();                                 //!< in Mops


            virtual ~number() throw(); //!< cleanup

        protected:
            explicit number() throw();                     //!< setup
            void     cast_overflow(const char *who) const; //!< rise exception

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(number);
        };
    }

}

#endif
