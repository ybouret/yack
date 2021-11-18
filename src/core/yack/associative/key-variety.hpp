
//! \file

#ifndef YACK_ASSOCIATIVE_KEY_VARIETY_INCLUDED
#define YACK_ASSOCIATIVE_KEY_VARIETY_INCLUDED 1

#include "yack/type/conversion.hpp"
#include "yack/type/traits.hpp"
#include "yack/memory/buffer/ro.hpp"

namespace yack
{
    
    struct key_variety
    {
        enum type
        {
            is_integral_type,
            is_legacy_string,
            is_memory_buffer,
            is_not_available
        };
        
        static const char *text(const type) throw();
        
        template <typename KEY>
        struct cull
        {
            typedef typename type_traits<KEY>::mutable_type mutable_key;
            static const bool _legacy_string  = is_same_type<mutable_key,char *>::value;
            static const bool _integral_type  = type_traits<mutable_key>::is_primitive;
            static const bool _memory_buffer  = YACK_IS_SUPERSUBCLASS(memory::ro_buffer,KEY);
            
            static const type variety =
            (   _legacy_string ? is_legacy_string :
             (  _integral_type ? is_integral_type :
              ( _memory_buffer ? is_memory_buffer : is_not_available)));
            
            
        };
        
    };
    
    
    
    
    
}

#endif

