#include "yack/jive/syntax/xnode.hpp"
#include "yack/jive/syntax/rule/internal.hpp"
#include "yack/jive/syntax/rule/terminal.hpp"


namespace yack
{
    namespace jive
    {
        namespace syntax
        {

            xnode::list:: list() throw() : list_() {}

            xnode::list:: ~list() throw()
            {
            }



            xnode:: ~xnode() throw()
            {
                if(data)
                {
                    switch( (**this).type )
                    {
                        case terminal_type:
                            delete static_cast<lexeme*>(data);
                            break;

                        case internal_type:
                            delete static_cast<xlist*>(data);
                            break;
                    }

                    data = 0;
                }
            }

            xnode:: xnode(const internal &r) :
            auth(r),
            next(0),
            prev(0),
            data( new xlist() )
            {

            }

            xnode:: xnode(const terminal &r, lexeme *lx) throw():
            auth(r),
            next(0),
            prev(0),
            data(lx)
            {
                assert(data);
            }

            xnode * xnode:: make(const internal &r)
            {
                return new xnode(r);
            }

            xnode * xnode:: make(const terminal &r, lexeme *lx)
            {
                assert(lx);
                auto_ptr<lexeme> g(lx);
                xnode           *p = new xnode(r,lx);
                g.dismiss();
                return p;
            }
            
            void xnode:: grow( xnode * &tree, xnode *node) throw()
            {
                assert(node);
                if(NULL==tree)
                {
                    tree = node;
                }
                else
                {
                    assert( (**tree).type == internal_type );
                    tree->sub().push_back(node);
                }
            }

            void xnode:: grow( xnode * &tree, const internal &r )
            {
                grow( tree, make(r) );
            }

            void xnode:: grow( xnode * &tree, const terminal &r, lexeme *l )
            {
                grow( tree, make(r,l) );
            }

            lexeme & xnode:: lex() throw()
            {
                assert(terminal_type==(**this).type);
                return *static_cast<lexeme *>(data);
            }

            const lexeme & xnode:: lex() const throw()
            {
                assert(terminal_type==(**this).type);
                return *static_cast<const lexeme *>(data);
            }

            list_of<xnode> & xnode:: sub()       throw()
            {
                assert(internal_type==(**this).type);
                return *static_cast<xlist *>(data);
            }

            const list_of<xnode> & xnode:: sub() const throw()
            {
                assert(internal_type==(**this).type);
                return *static_cast<const xlist *>(data);
            }

            lexeme * xnode::rip() throw()
            {
                assert(terminal_type==(**this).type);
                lexeme *res = static_cast<lexeme *>(data);
                data = 0;
                delete this;
                return res;
            }

            static inline void xlist_ret(lexer &L, list_of<xnode> &chld) throw()
            {
                while(chld.size)
                {
                    xnode::ret(L,chld.pop_back());
                }
            }

            void xnode::ret(lexer &L, xnode *node) throw()
            {
                assert(node);
                switch( (**node).type )
                {
                    case internal_type:
                        std::cerr << "\t\t\tret '" << (**node).name << "'" << std::endl;
                        xlist_ret(L,node->sub());
                        delete node;
                        break;

                    case terminal_type:
                        std::cerr << "\t\t\tret '" << *(node->lex()) << "'" << std::endl;
                        L.store( node->rip() );
                        break;
                }
            }


            void xnode:: gv_(ios::ostream &fp)   const
            {
                const rule &r = **this;

                logo(fp);
                string l = *r.name;
                switch(r.type)
                {
                    case internal_type: {
                        // <internal>
                        fp << '[';
                        add_label(fp,l());
                        fp << ",shape=house";
                        fp << ']';
                        end(fp);
                        const list_of<xnode> &chld = sub();
                        const bool            show = chld.size>1;
                        unsigned              indx = 1;
                        for(const xnode *node=chld.head;node;node=node->next,++indx)
                        {
                            node->gv_(fp);
                            link(fp,node);
                            if(show)
                            {
                                fp("[label=\"%u\"]",indx);
                            }
                            end(fp);
                        }
                        // <internal/>
                    } break;

                    case terminal_type: {
                        // <terminal>
                        const terminal &t = *r.as<terminal>();
                        switch(t.role)
                        {
                            case standard:
                                l += " = '";
                                l += (*lex()).to_string();
                                l += "'";
                                fp << '[';
                                add_label(fp,l());
                                fp << ",shape=box";
                                fp << ']';
                                break;

                            case univocal:
                                fp << '[';
                                add_label(fp,l());
                                fp << ",shape=box,style=rounded";
                                fp << ']';
                                break;

                            case division:
                                fp << '[';
                                add_label(fp,l());
                                fp << ",shape=box,style=dashed";
                                fp << ']';
                                break;
                        }
                        end(fp);
                        // <terminal/>
                    } break;
                }

            }

            void xnode:: gv(const string &filename) const
            {
                {
                    ios::ocstream fp(filename);
                    digraph_init(fp,"G");
                    gv_(fp);
                    digraph_quit(fp);
                }

                ios::vizible::render(filename);
            }

            void xnode:: gv(const char *filename) const
            {
                const string _(filename);
                gv(_);
            }


        }
    }
}
