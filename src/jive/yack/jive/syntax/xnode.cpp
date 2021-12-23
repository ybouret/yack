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
                    switch( (**this).kind )
                    {
                        case is_terminal:
                            break;

                        case is_internal:
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

            xnode * xnode:: create(const internal &r)
            {
                return new xnode(r);
            }

            xnode * xnode:: create(const terminal &r, lexeme *lx)
            {
                assert(lx);
                auto_ptr<lexeme> g(lx);
                xnode           *p = new xnode(r,lx);
                g.dismiss();
                return p;
            }
            

        }

    }
}
