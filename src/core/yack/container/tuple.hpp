//! \file

#ifndef YACK_TUPLE_INCLUDED
#define YACK_TUPLE_INCLUDED 1

#include "yack/setup.hpp"
#include <iostream>

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! aliases for standard tuples
    //
    //__________________________________________________________________________
#define YACK_TUPLE_ARGS_STANDARD(T,NAME)                                       \
/**/    typedef T                               NAME;                          \
/**/    typedef type_traits<T>::mutable_type    mutable_##NAME;                \
/**/    typedef const mutable_##NAME              const_##NAME;                \
/**/    typedef                                                                \
/**/        yack::pick_from<                                                   \
/**/            type_traits<mutable_##NAME>::is_primitive,                     \
/**/            const_##NAME  ,                                                \
/**/            const_##NAME &                                                 \
/**/        >::type param_##NAME

    //__________________________________________________________________________
    //
    //
    //! aliases for template tuples
    //
    //__________________________________________________________________________
#define YACK_TUPLE_ARGS_TEMPLATE(T,NAME)\
/**/    typedef T                                        NAME;                 \
/**/    typedef typename type_traits<T>::mutable_type    mutable_##NAME;       \
/**/    typedef const mutable_##NAME                     const_##NAME;         \
/**/    typedef typename                                                       \
/**/        yack::pick_from<                                                   \
/**/            type_traits<mutable_##NAME>::is_primitive,                     \
/**/            const_##NAME  ,                                                \
/**/            const_##NAME &                                                 \
/**/        >::type param_##NAME


}

#endif
