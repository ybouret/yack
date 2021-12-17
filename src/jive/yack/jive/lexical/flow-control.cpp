#include "yack/jive/lexical/flow-control.hpp"
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
                
                
                jump:: jump(const tag      &T,
                            analyzer       &A,
                            const callback &C) :
                branching(A,C),
                to(T)
                {
                }
                
                jump:: ~jump() throw()
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

