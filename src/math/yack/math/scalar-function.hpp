
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
        class scalar_function
        {
        public:
            virtual ~scalar_function() throw();
            
            virtual T operator()(T) = 0;
            
        protected:
            explicit scalar_function() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(scalar_function);
            
        public:
            template <typename FUNC>
            class call : public scalar_function
            {
            public:
                inline explicit call(FUNC &func) throw() : host(func) {}
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

