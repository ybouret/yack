
#include "yack/jive/lexical/flow/call.hpp"

namespace yack
{
    namespace jive
    {
        
        namespace lexical
        {
            
            namespace flow
            {
                
                
                call:: call(const tag      &T,
                            analyzer       &A,
                            const callback &C) :
                branching(A,C),
                to(T)
                {
                }
                
                call:: ~call() noexcept
                {
                }
                
                call:: call(const call &_) : branching(_), to(_.to)
                {
                }
                
                const char call::text[] = "=>";
                
            }
            
        }
        
    }
    
}

