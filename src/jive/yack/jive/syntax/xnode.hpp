//! \file

#ifndef YACK_JIVE_SYNTAX_XNODE_INCLUDED
#define YACK_JIVE_SYNTAX_XNODE_INCLUDED 1

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
            class xnode :
            public object,
            public collection,
            public authority<const rule>,
            public ios::vizible,
            public ios::serializable
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef authority<const rule> auth;   //!< alias
                typedef cxx_list_of<xnode>    list_;  //!< base class for xlist
                static const char             clid[]; //!< "xnode"

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
                static xnode *make(const internal &);                           //!< create data=xlist
                static xnode *make(const terminal &, lexeme *);                 //!< set    data=lexeme
                static void   grow( xnode * &tree, xnode *node) throw();        //!< grow tree
                static void   grow( xnode * &tree, const internal &);           //!< grow(tree,make(internal))
                static void   grow( xnode * &tree, const terminal &, lexeme *); //!< grow(tree,make(terminal,lexeme))
                static void   join( xnode * &tree, xnode *node) throw();        //!< set tree or merge content

                lexeme               &lex()       throw(); //!< return content  for terminal
                const lexeme         &lex() const throw(); //!< return content  for terminal
                list                 &sub()       throw(); //!< retun  children for internal
                const list           &sub() const throw(); //!< return children for internal

                lexeme               *rip() throw();                //!< extract lexeme, delete this
                static void           ret(lexer &,xnode *) throw(); //!< return node to lexer, node is deleted
                void                  gv(const string &)     const; //!< save/render graphviz
                void                  gv(const char   *)     const; //!< save/render graphviz
                
                const xnode           *head() const throw(); //!< first children for internal
                const token           &word() const throw(); //!< return token   for terminal
                virtual size_t         size() const throw(); //!< internal: list size, terminal: word.size
                
                //______________________________________________________________
                //
                //! concrete tree to abstract tree, may return NULL
                //______________________________________________________________
                static xnode         *ast(xnode *node) throw();

                //______________________________________________________________
                //
                // serializable interface
                //______________________________________________________________
                virtual const char * class_uid() const throw();
                virtual size_t       serialize(ios::ostream &) const;

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
                void     gv_(ios::ostream &)                 const; //!< write self, child, link

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

