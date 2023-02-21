#include "yack/jive/pattern/basic/single.hpp"
#include "yack/jive/pattern/first-bytes.hpp"
#include "yack/ios/ascii/hybrid.hpp"

namespace yack
{
    namespace jive
    {
        const char single::clid[] = "single";

        const char * single:: class_uid() const noexcept { return clid; }

        single:: ~single() noexcept
        {
        }

        single:: single(const uint8_t ch) noexcept :
        pattern(mark),
        code(ch)
        {
            I_am<single>();
        }

        bool single:: is_same_than(const single *other) const noexcept
        {
            assert(other);
            return code == other->code;
        }

        std::ostream & operator<<(std::ostream &os,const single &p)
        {
            os << '\'' << ios::ascii::hybrid[p.code] << '\'';
            return os;
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

        void single:: firsts(first_bytes &fc) const
        {
            fc << new domain(code);
        }

        void single:: encode(ios::ostream &fp) const
        {
            logo(fp) << "[";
            fp << "label=\"'"; text(fp,code); fp << "'\"";
            fp << ",shape=square";
            fp << "]";
            end(fp);
        }

        pattern * single::clone() const
        {
            return new single(code);
        }

        bool single:: is_univocal() const noexcept
        {
            return true;
        }
    }

}

