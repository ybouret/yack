
//! \file
#ifndef YACK_TYPE_CONVERSION_INCLUDED
#define YACK_TYPE_CONVERSION_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    //! class conversion prototype
    template <class T,class U>
    class conversion {
    private:
        typedef uint8_t small_type;                      //!< small type alias
        class           large_type { uint32_t unused; }; //! inner large type

        static  small_type test( U ); //!< returned type for U
        static  large_type test(...); //!< returned type for other U
        static  T     makeT();        //!< a prototype for returning T

    public:
        conversion(); //!< or some compiler will complain about all private...
        enum
        {
            exists   = ( sizeof( test(makeT()) ) == sizeof(small_type) ), //!< type conversion is possible
            same_type = false                                             //!< with different types!
        };
    };

    //! specialized self conversion
    template <class T >
    class conversion<T,T> {
    public:
        enum {
            exists    = true, //!< obvious
            same_type = true  //!< obvious
        };
    };

    //! true if U is derived from T, may be T
#define YACK_IS_SUPERSUBCLASS(T,U) \
(conversion<const U *,const T *>::exists && (!conversion<const T *, const void *>::same_type) )

    //! true is U is derived from T, and not T
#define YACK_IS_SUPERSUBCLASS_STRICT(T,U) \
( YACK_IS_SUPERSUBCLASS(T,U) && (!conversion<const T *,const U *>::same_type) )

}


#endif
