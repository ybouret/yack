
//! \file

#ifndef YACK_MATH_SCALAR_FUNCTION_INCLUDED
#define YACK_MATH_SCALAR_FUNCTION_INCLUDED 1

#include "yack/type/utils.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {

        //______________________________________________________________________
        //
        //
        //! real function interface
        //
        //______________________________________________________________________
        template <typename T>
        class real_function
        {
        public:
            virtual ~real_function() throw(); //!< cleanup
            virtual T operator()(T)      = 0; //!< call operator
            
        protected:
            explicit real_function() throw(); //!< setup
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(real_function);

        };

        //______________________________________________________________________
        //
        //
        //! wrapper for any persitent real function (CFunction,functor,class...)
        //
        //______________________________________________________________________
        template <typename T>
        struct real_function_of
        {
            //__________________________________________________________________
            //
            //! forwarding call to make a real function behavior
            //__________________________________________________________________
            template <typename FUNC>
            class call : public real_function<T>
            {
            public:
                inline explicit call(FUNC &func) throw() : real_function<T>(), host(func) {}   //! setup
                inline virtual ~call()           throw() {}                                    //!< cleanup
                inline virtual T operator()(T x) { return host(x); }                           //!< forward call

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(call);
                FUNC &host;
            };

        };

    }
    
}

#endif

