#include "yack/jive/lexical/flow/branching.hpp"
namespace yack
{
    namespace jive
    {
        
        
        namespace lexical
        {
            
            namespace flow
            {

                branching:: ~branching() throw()
                {
                }
                
                branching:: branching(analyzer       &A,
                                      const callback &C) :
                az(A), cb(C)
                {
                }
                
                branching:: branching(const branching &_) :
                az(_.az),
                cb(_.cb)
                {
                }
                
            }
            
        }
        
    }
    
}

