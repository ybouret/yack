#include "yack/jive/pattern/joker/counting.hpp"

#include "yack/jive/pattern/joker/at-least.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/ios/encoder.hpp"

namespace yack
{
    namespace jive
    {
        const char   counting:: clid[] = "counting";
        const char * counting:: class_uid() const throw() { return clid; }

        counting:: ~counting() throw()
        {
        }

        counting:: counting(const size_t n, const size_t m, pattern *p) throw() :
        joker(mark,p),
        nmin(n),
        nmax(m)
        {
            if(nmax<nmin) coerce_cswap(n,m);
            I_am<counting>();
        }

        bool counting:: strong() const
        {
            return (nmin>0) && (**this).strong();
        }

        bool  counting:: accept(source &src, token &tkn) const
        {
            size_t num = 0;
            while(true)
            {
                token  sub;
                if( (**this).accept(src,sub) )
                {
                    tkn.merge_back(sub);
                    ++num;
                    continue;
                }
            }
            if(num>=nmin&&num<=nmax)
            {
                return true;
            }
            else
            {
                src.store(tkn);
                return false;
            }
        }

        pattern * counting::create(const size_t n, const size_t m, pattern *p)
        {
            assert(p);
            auto_ptr<pattern> k(p);
            pattern          *j = new counting(n,m,p);
            k.dismiss();
            return j;
        }

        size_t counting:: serialize(ios::ostream &fp) const
        {
            size_t nw = emit_uuid(fp);
            nw       += ios::encoder::serialize(fp,nmin);
            nw       += ios::encoder::serialize(fp,nmax);
            return nw + (**this).serialize(fp);
        }

        void counting:: encode(ios::ostream &fp) const
        {
            tag(fp) << "[";
            fp << "label=\"{";
            fp("%u:%u", unsigned(nmin), unsigned(nmax));
            fp << "}\"";
            fp << ",shape=octagon";
            fp << "]";
            end(fp);
            viz(fp);
        }

    }

}

 
