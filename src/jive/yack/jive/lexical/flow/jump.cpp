#include "yack/jive/lexical/flow/jump.hpp"

namespace yack
{
    namespace jive
    {
        
        namespace lexical
        {
            
            namespace flow
            {
                
                
                jump:: jump(const tag      &T,
                            analyzer       &A,
                            const callback &C) :
                branching(A,C),
                to(T)
                {
                }
                
                jump:: ~jump() noexcept
                {
                }
                
                jump:: jump(const jump &_) : branching(_), to(_.to)
                {
                }
                
                const char jump::text[] = "->";
                
            }
            
        }
        
    }
    
}

