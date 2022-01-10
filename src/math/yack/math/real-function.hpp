
//! \file

#ifndef YACK_MATH_SCALAR_FUNCTION_INCLUDED
#define YACK_MATH_SCALAR_FUNCTION_INCLUDED 1

#include "yack/type/utils.hpp"
#include <cmath>

namespace yack
{
    namespace math
    {
        
        template <typename T>
        class real_function
        {
        public:
            virtual ~real_function() throw();
            
            virtual T operator()(T) = 0;
            
        protected:
            explicit real_function() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(real_function);
            
        public:
            template <typename FUNC>
            class call : public real_function
            {
            public:
                inline explicit call(FUNC &func) throw() : real_function(), host(func) {}
                inline virtual ~call() throw() {}
                
                inline virtual T operator()(T x)
                {
                    return host(x);
                }
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(call);
                FUNC &host;
            };
        };
    }
    
}

#endif

