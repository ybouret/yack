#include "yack/jive/lexical/analyzer.hpp"

namespace yack
{
    
    namespace jive
    {
        
        namespace lexical
        {
            namespace flow
            {
                behavior jump:: operator()(token &word)
                {
                    cb(word);
                    az.flow_jump(*to);
                    return control;
                }
                
                behavior call:: operator()(token &word)
                {
                    cb(word);
                    az.flow_call(*to);
                    return control;
                }
                
                behavior back::operator()(token &word)
                {
                    cb(word);
                    az.flow_back();
                    return control;
                }
                
                
            }
        }
        
    }
    
}

