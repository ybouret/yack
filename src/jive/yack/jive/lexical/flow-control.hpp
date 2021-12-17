

#ifndef YACK_LEXICAL_FLOW_CONTROL_INCLUDED
#define YACK_LEXICAL_FLOW_CONTROL_INCLUDED 1


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
                    virtual ~branching() throw();
                    explicit branching(analyzer       &,
                                       const callback &);
                    explicit branching(const branching &);
                    
                    analyzer &az; //!< target
                    callback  cb; //!< init/quit upon branchin
                    
                    
                private:
                    YACK_DISABLE_ASSIGN(branching);
                };
                
                class jump : public branching
                {
                public:
                    static const char text[];
                    virtual ~jump() throw();
                    explicit jump(const tag      &,
                                  analyzer       &,
                                  const callback &);
                    explicit jump(const jump &);
                    
                    const tag to;
                    
                    behavior operator()(token &);
                    
                private:
                    YACK_DISABLE_ASSIGN(jump);
                };
                
            }
            
        }
    }
    
}

#endif
