//! \file
#ifndef YACK_TYPE_ARGS_INCLUDED
#define YACK_TYPE_ARGS_INCLUDED 1

#include "yack/type/traits.hpp"

namespace yack {


    //! container like aliases
#define YACK_DECL_ARGS_(T,NAME)                                               \
typedef T                                        NAME;                        \
typedef typename type_traits<T>::mutable_type    mutable_##NAME;              \
typedef const mutable_##NAME                     const_##NAME

    //! container like aliases and parameter
    /**
     * streams, containers,..., can't contain references,
     * so these are the aliases used for in/out parameters passing
     */
#define    YACKDECL_ARGS(T,NAME)                                              \
Y_DECL_ARGS_(T,NAME);                                                         \
typedef typename                                                              \
upsylon::select_type<                                                         \
type_traits<mutable_##NAME>::is_primitive, /* no references in containers */  \
const_##NAME  , /* pass primitive mutable types by const value            */  \
const_##NAME &  /* pass non primitive mutable types by const reference    */  \
>::result param_##NAME
    


}

#endif

