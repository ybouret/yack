//! \file

#ifndef YACK_APEX_TYPES_INCLUDED
#define YACK_APEX_TYPES_INCLUDED 1

#include "yack/type/ints.hpp"

namespace yack
{

    namespace apex
    {

        typedef uint64_t unsigned_type; //!< integral unigned type
        typedef int64_t  signed_type;   //!< integral signed type

        #define YACK_APEX_TYPE uint16_t
        //#define YACK_APEX_TYPE uint32_t

#if !defined(YACK_APEX_TYPE)
#     define YACK_APEX_TYPE void * //!< default matching type
#endif

        //______________________________________________________________________
        //
        //
        //! computation of working types
        //
        //______________________________________________________________________
        struct cull
        {
            // native computation
            typedef unsigned_for<YACK_APEX_TYPE>::type core_type; //!< native type
            static const size_t core_size = sizeof(core_type);    //!< native type size
            static const size_t core_bits = core_size << 3;       //!< native type bits

            // internal storage
            static const size_t word_size = core_size >> 1;       //!< internal type size
            static const size_t word_bits = core_bits >> 1;       //!< internal type bits
            typedef unsigned_int<word_size>::type word_type;      //!< internal type

        };



    }

}

#endif
