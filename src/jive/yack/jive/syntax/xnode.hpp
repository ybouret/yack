//! \file

#ifndef YACK_JIVE_SYNTAX_XNODE_INCLUDED
#define YACK_JIVE_SYNTAX_XNODE_INCLUDED 1

#include "yack/jive/lexical/lexeme.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            
            class xnode : public object
            {
            public:
                enum family
                {
                    is_terminal,
                    is_internal
                };
                typedef cxx_list_of<xnode> list_;
                
                class list : public object, public list_
                {
                public:
                    virtual ~list() throw();
                    explicit list() throw();

                private:
                    YACK_DISABLE_COPY_AND_ASSIGN(list);
                };

                xnode *next;
                xnode *prev;
                
                virtual ~xnode() throw();


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(xnode);
            };

            typedef xnode::list xlist;
            
        }
    }
}

#endif

