//! \file

#ifndef YACK_LEXICAL_FLOW_BRANCHING_INCLUDED
#define YACK_LEXICAL_FLOW_BRANCHING_INCLUDED 1


#include "yack/jive/tags.hpp"
#include "yack/jive/lexical/behavior.hpp"
#include "yack/functor.hpp"

namespace yack
{
    namespace jive
    {
        
        class token;
        
        namespace lexical
        {
            class analyzer;
            
            namespace flow
            {
                typedef functor<void,TL1(token&)> callback; //!< alias
                
                //______________________________________________________________
                //
                //
                //! base class for jump/call/back
                //
                //______________________________________________________________
                class branching
                {
                public:
                    //__________________________________________________________
                    //
                    // members
                    //__________________________________________________________
                    analyzer &az; //!< target to modify
                    callback  cb; //!< init/quit upon branchin
                                  
                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    virtual ~branching() noexcept;          //!< cleanup
                    
                protected:
                    explicit branching(analyzer       &,
                                       const callback &); //!< setup
                    explicit branching(const branching &); //!< copy
                                                            
                private:
                    YACK_DISABLE_ASSIGN(branching);
                };
                
               
                
              
                
            }
            
        }
    }
    
}

#endif
