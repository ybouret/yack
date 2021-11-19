
//! \file

#ifndef YACK_ASSOCIATIVE_KEY_VARIETY_INCLUDED
#define YACK_ASSOCIATIVE_KEY_VARIETY_INCLUDED 1

#include "yack/type/conversion.hpp"
#include "yack/type/traits.hpp"
#include "yack/memory/buffer/ro.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! defining key variety for hasher/walker
    //
    //__________________________________________________________________________
    struct key_variety
    {
        //______________________________________________________________________
        //
        //! type definitions
        //______________________________________________________________________
        enum type
        {
            is_integral_type, //!< will use data
            is_legacy_string, //!< [const] char (*|[])
            is_memory_buffer, //!< derived from memory::ro_buffer
            is_not_available  //!< cannot be used as key
        };

        //______________________________________________________________________
        //
        //! named type
        //______________________________________________________________________
        static const char *text(const type) throw();

        //______________________________________________________________________
        //
        //! KEY type culling
        //______________________________________________________________________
        template <typename KEY>
        struct cull
        {
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef typename   type_traits<KEY>::mutable_type mutable_key;                                                      //!< alias
            static  const bool _legacy_string  = is_same_type<mutable_key,char *>::value || type_traits<mutable_key>::is_array; //!< legacy string flag
            static  const bool _integral_type  = type_traits<mutable_key>::is_primitive;                                        //!< integral type flag
            static  const bool _memory_buffer  = YACK_IS_SUPERSUBCLASS(memory::ro_buffer,KEY);                                  //!< derived buffer flag

            //__________________________________________________________________
            //
            //! define type using priorities
            //__________________________________________________________________
            static const type value =
            (   _legacy_string ? is_legacy_string :
             (  _integral_type ? is_integral_type :
              ( _memory_buffer ? is_memory_buffer : is_not_available)));
        };
        
        typedef int2type<is_integral_type> integral_type; //!< alias
        typedef int2type<is_legacy_string> legacy_string; //!< alias
        typedef int2type<is_memory_buffer> memory_buffer; //!< alias

    };
    
    
    
    
    
    
}

#endif

