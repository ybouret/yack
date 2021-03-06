
#include "yack/jive/pattern/regexp/compiler.hpp"
#include "yack/jive/pattern/all.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace jive
    {
        void RXCompiler:: joker(logical &p, const char j) const
        {
            if(p.size<=0)
            {
                throw exception("%s: no expression before '%c' in '%s'",clid,j,expr);
            }
            switch(j)
            {
                case zom: p.push_back( zero_or_more(p.pop_back()) );        break;
                case oom: p.push_back( one_or_more(p.pop_back()) );         break;
                case opt: p.push_back( optional::create(p.pop_back()));     break;
                case ign: p.push_back( pattern::ignore_case(p.pop_back())); break;
                case Not: {
                    auto_ptr<logical> op = new op_none();
                    op->push_back(p.pop_back());
                    p.push_back( pattern::optimize(op.yield()) );
                } break;
                default:
                    throw exception("%s: unhandled joker '%c' in '%s'",clid,j,expr);
            }
        }
    }

}

