//! \file

#ifndef YACK_LEXICAL_FLOW_CALL_INCLUDED
#define YACK_LEXICAL_FLOW_CALL_INCLUDED 1


#include "yack/jive/lexical/flow/branching.hpp"

namespace yack
{
    namespace jive
    {
        namespace lexical
        {
            
            namespace flow
            {
                //______________________________________________________________
                //
                //
                //! make a call to scanner
                //
                //______________________________________________________________
                class call : public branching
                {
                public:
                    //__________________________________________________________
                    //
                    // types and definitions
                    //__________________________________________________________
                    static const char text[]; //!< to create labels
                    
                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    virtual ~call() noexcept;          //!< cleanup
                    explicit call(const tag      &,
                                  analyzer       &,
                                  const callback &);  //!< setup
                    explicit call(const call &);      //!< copy
                    
                    //__________________________________________________________
                    //
                    //! change analyzer's scanner and perform callback
                    //__________________________________________________________
                    behavior operator()(token &);
                    
                    //__________________________________________________________
                    //
                    // members
                    //__________________________________________________________
                    const tag to; //!< scanner to jump to
                    
                    
                private:
                    YACK_DISABLE_ASSIGN(call);
                };
            }
        }
    }
}

#endif

