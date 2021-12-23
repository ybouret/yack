//! \file

#ifndef YACK_JIVE_SYNTAX_XNODE_INCLUDED
#define YACK_JIVE_SYNTAX_XNODE_INCLUDED 1

#include "yack/jive/lexical/lexeme.hpp"
#include "yack/jive/syntax/rule.hpp"
#include "yack/type/authority.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {


            class xnode : public object, public authority<const rule>
            {
            public:

                typedef cxx_list_of<xnode> list_;
                
                class list : public object, public list_
                {
                public:
                    virtual ~list() throw();
                    explicit list() throw();

                private:
                    YACK_DISABLE_COPY_AND_ASSIGN(list);
                };

                xnode       *next;
                xnode       *prev;
                
                virtual ~xnode() throw();
                explicit xnode(const rule &); //!< internal

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(xnode);
                void *data;
            };

            typedef xnode::list xlist;
            
        }
    }
}

#endif

