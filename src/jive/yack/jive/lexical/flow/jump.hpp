
//! \file

#ifndef YACK_LEXICAL_FLOW_JUMP_INCLUDED
#define YACK_LEXICAL_FLOW_JUMP_INCLUDED 1


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
                //! make a change of scanner
                //
                //______________________________________________________________
                class jump : public branching
                {
                public:
                    //__________________________________________________________
                    //
                    // types and definitions
                    //__________________________________________________________
                    static const char text[]; //!< for create labels
                    
                    //__________________________________________________________
                    //
                    // C++
                    //__________________________________________________________
                    virtual ~jump() throw();          //!< cleanup
                    explicit jump(const tag      &,
                                  analyzer       &,
                                  const callback &);  //!< setup
                    explicit jump(const jump &);      //!< copy
                    
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
                    YACK_DISABLE_ASSIGN(jump);
                };
            }
        }
    }
}

#endif

