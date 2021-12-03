
#include "yack/jive/pattern/basic/except.hpp"
#include "yack/jive/pattern/first-bytes.hpp"

namespace yack
{
    namespace jive
    {
        const char except::clid[] = "except";

        const char * except:: class_uid() const throw() { return clid; }

        except:: ~except() throw()
        {
        }

        except:: except(const uint8_t ch) throw() :
        pattern(mark),
        code(ch)
        {
            I_am<except>();
        }

        size_t except:: serialize(ios::ostream &fp) const
        {
            size_t nw = emit_uuid(fp);
            fp.write(code);
            return ++nw;
        }

        bool except:: strong() const
        {
            return true;
        }

        bool except:: accept(source &src, token &tkn) const
        {
            assert(0==tkn.size);
            character *ch = src.query();
            if(ch)
            {
                if(code!=**ch)
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

        void except:: firsts(first_bytes &fc) const
        {
            first_bytes sub;
            sub.all();
            sub -= code;
            fc << sub;
        }

        bool except:: is_same_than(const except *other) const throw()
        {
            assert(other);
            return code==other->code;
        }

        void except:: encode(ios::ostream &fp) const
        {
            logo(fp) << "[";
            fp << "label=\"'"; text(fp,code); fp << "'\"";
            fp << ",shape=triangle";
            fp << "]";
            end(fp);
        }

    }

}

