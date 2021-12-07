
#include "yack/jive/pattern/all.hpp"
#include "yack/jive/pattern/first-bytes.hpp"


namespace yack
{
    namespace jive
    {

        static inline pattern *make_couple(pattern *p, const uint8_t c)
        {
            auto_ptr<pattern> guard(p);
            auto_ptr<logical> res = new op_or();
            res->push_back( guard.yield() );
            res->push_back( new single(c) );
            return res.yield();
        }

        static inline pattern *ign_build(single *s)
        {
            const int lhs = s->code;
            if( isupper(lhs) )
            {
                return make_couple(s,tolower(lhs));
            }
            else
            {

                if( islower(lhs) )
                {
                    return make_couple(s,toupper(lhs));
                }
                else
                {
                    return s;
                }
            }
        }

        static inline pattern *ign_build(within *w)
        {
            auto_ptr<pattern> guard(w);
            first_bytes       fc;
            const int         lo = w->lower;
            const int         up = w->upper;

            patterns si;
            patterns in;
            for(int ch=lo;ch<=up;++ch)
            {
                fc << new domain(ch);
                if(islower(ch)) fc << new domain(toupper(ch));
                if(isupper(ch)) fc << new domain(tolower(ch));
            }

            for(const domain *dom = fc.head;dom;dom=dom->next)
            {
                if(dom->lower>=dom->upper)
                {
                    si.push_back( new single(dom->lower) );
                }
                else
                {
                    in.push_back( new within(dom->lower,dom->upper) );
                }
            }

            logical *p = new op_or();
            p->merge_back(si);
            p->merge_back(in);
            return p;

        }


        pattern * pattern:: ignore_case(pattern *p)
        {
            switch(p->uuid)
            {
                case single::mark: return ign_build(p->as<single>());
                case within::mark: return ign_build(p->as<within>());
                default:
                    break;
            }

            return p;

        }
    }

}

