#include "yack/jive/pattern/basic/single.hpp"

namespace yack
{
    namespace jive
    {
        const char single::clid[] = "single";

        const char * single:: class_uid() const throw() { return clid; }

        single:: ~single() throw()
        {
        }

        single:: single(const uint8_t ch) throw() :
        pattern(mark),
        code(ch)
        {
            I_am<single>();
        }

        size_t single:: serialize(ios::ostream &fp) const
        {
            size_t nw = emit_uuid(fp);
            fp.write(code);
            return ++nw;
        }

        bool single:: strong() const
        {
            return true;
        }

       bool single:: accept(source &src, token &tkn) const
        {
            assert(0==tkn.size);
            character *ch = src.query();
            if(ch)
            {
                if(code==**ch)
                {
                    // match
                    tkn.push_back(ch);
                    return true;
                }
                else
                {
                    // no match
                    src.store(ch);
                    return false;
                }
            }
            else
            {
                // EOF
                return false;
            }
        }

    }

}

