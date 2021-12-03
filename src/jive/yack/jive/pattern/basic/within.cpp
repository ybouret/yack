#include "yack/jive/pattern/basic/within.hpp"
#include "yack/jive/pattern/first-bytes.hpp"

namespace yack
{
    namespace jive
    {
        const char within::clid[] = "within";

        const char * within:: class_uid() const throw() { return clid; }

        within:: ~within() throw()
        {
        }

        within:: within(const uint8_t lo, const uint8_t up) throw() :
        pattern(mark),
        lower(lo),
        upper(up)
        {
            if(lower>upper)
            {
                coerce_cswap(lower,upper);
            }
            I_am<within>();
        }

        size_t within:: serialize(ios::ostream &fp) const
        {
            size_t nw = emit_uuid(fp);
            fp.write(lower);
            fp.write(upper);
            return 2+nw;
        }

        bool within:: strong() const
        {
            return true;
        }

        bool within:: accept(source &src, token &tkn) const
        {
            assert(0==tkn.size);
            character *ch = src.query();
            if(ch)
            {
                const uint8_t value = **ch;
                if(lower<=value&&value<=upper)
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

        void within:: firsts(first_bytes &fc) const
        {
            fc << new domain(lower,upper);
        }

        bool within:: is_same_than(const within *other) const throw()
        {
            assert(other);
            return (lower==other->lower) && (upper==other->upper);
        }

        void within:: encode(ios::ostream &fp) const
        {
            tag(fp) << "[";
            fp << "label=\"'"; text(fp,lower); fp << "'-'"; text(fp,upper); fp << "'\"";
            fp << ",shape=box";
            fp << "]";
            end(fp);
        }
        
    }

}

