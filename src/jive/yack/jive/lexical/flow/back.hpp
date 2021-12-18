//! \file

#ifndef YACK_LEXICAL_FLOW_BACK_INCLUDED
#define YACK_LEXICAL_FLOW_BACK_INCLUDED 1


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
                //! return from scanner
                //
                //______________________________________________________________
                class back : public branching
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
                    virtual ~back() throw();          //!< cleanup
                    explicit back(analyzer       &,
                                  const callback &);  //!< setup
                    explicit back(const back     &);  //!< copy
                    
                    //__________________________________________________________
                    //
                    //! change analyzer's scanner and perform callback
                    //__________________________________________________________
                    behavior operator()(token &);
                    
                    
                    
                private:
                    YACK_DISABLE_ASSIGN(back);
                };
            }
        }
    }
}

#endif

