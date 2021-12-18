
#include "yack/jive/lexical/flow/back.hpp"

namespace yack
{
    namespace jive
    {
        
        namespace lexical
        {
            
            namespace flow
            {
                
                
                back:: back(analyzer       &A,
                            const callback &C) :
                branching(A,C)
                {
                }
                
                back:: ~back() throw()
                {
                }
                
                back:: back(const back &_) : branching(_)
                {
                }
                
                const char back::text[] = "<==";
                
            }
            
        }
        
    }
    
}

