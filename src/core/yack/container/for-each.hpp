//! \file

#ifndef YACK_FOR_EACH_INCLUDED
#define YACK_FOR_EACH_INCLUDED 1

#include "yack/type/args.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! helpers to apply to ranges
    //
    //__________________________________________________________________________
    struct for_each
    {
        //______________________________________________________________________
        //
        //! check data inside based on operator==
        //______________________________________________________________________
        template <typename T>
        struct has
        {
            //! alias
            typedef typename type_traits<T>::parameter_type param_type;

            //! check in range
            template <typename ITERATOR> static inline
            bool in(ITERATOR       curr,
                    const ITERATOR last,
                    param_type     args)
            {
                while(curr!=last)
                {
                    if( *(curr++) == args ) return true;
                }
                return false;
            }

            //! check in full sequence
            template <typename SEQUENCE> static inline
            bool in(SEQUENCE  &seq,
                    param_type args)
            {
                return in( seq.begin(), seq.end(), args);
            }

        };


    };

}


#endif

