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

            class terminal;
            class internal;

            //__________________________________________________________________
            //
            //
            //! flexible node to hold the syntax tree
            //
            //__________________________________________________________________
            class xnode : public object, public authority<const rule>
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef authority<const rule> auth;  //!< alias
                typedef cxx_list_of<xnode>    list_; //!< base class for xlist

                //______________________________________________________________
                //
                //! xlist implementation
                //______________________________________________________________
                class list : public object, public list_
                {
                public:
                    virtual ~list() throw(); //!< cleanup
                    explicit list() throw(); //!< setup empty

                private:
                    YACK_DISABLE_COPY_AND_ASSIGN(list);
                };

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                static xnode *create(const internal &);           //!< create data=xlist
                static xnode *create(const terminal &, lexeme *); //!< set    data=lexeme

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                xnode       *next; //!< for xlist
                xnode       *prev; //!< for xlist

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~xnode() throw(); //!< cleanup

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(xnode);
                explicit xnode(const internal &);                   //!< internal
                explicit xnode(const terminal &, lexeme *) throw(); //!< terminal
                void    *data; //!< lexeme/xlist

            };

            //__________________________________________________________________
            //
            //
            //! internal list of xnodes
            //
            //__________________________________________________________________
            typedef xnode::list xlist;
            
        }
    }
}

#endif

