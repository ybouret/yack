
#include "yack/jive/pattern/all.hpp"
#include "yack/jive/pattern/first-bytes.hpp"


namespace yack
{
    namespace jive
    {
        //----------------------------------------------------------------------
        //
        // gather pattern and its counterpoint in a logical pattern
        //
        //----------------------------------------------------------------------
        template <typename OP>
        static inline pattern *make_couple(pattern *p, const uint8_t c)
        {
            auto_ptr<pattern> guard(p);
            auto_ptr<logical> res = new OP();
            res->push_back( guard.yield() );
            res->push_back( new single(c) );
            return res.yield();
        }

        //----------------------------------------------------------------------
        //
        // ignore case for single : couple OR if necessary
        //
        //----------------------------------------------------------------------
        static inline pattern *ign_build(single *s)
        {
            const int lhs = s->code;
            if( isupper(lhs) )
            {
                return make_couple<op_or>(s,tolower(lhs));
            }
            else
            {

                if( islower(lhs) )
                {
                    return make_couple<op_or>(s,toupper(lhs));
                }
                else
                {
                    return s;
                }
            }
        }

        //----------------------------------------------------------------------
        //
        // ignore case for except : couple NONE if necessary
        //
        //----------------------------------------------------------------------
        static inline pattern *ign_build(except *s)
        {
            const int lhs = s->code;
            if( isupper(lhs) )
            {
                return make_couple<op_none>(s,tolower(lhs));
            }
            else
            {

                if( islower(lhs) )
                {
                    return make_couple<op_none>(s,toupper(lhs));
                }
                else
                {
                    return s;
                }
            }
        }

        //----------------------------------------------------------------------
        //
        // ignore case for a range : use first_bytes to collect possibilities
        //
        //----------------------------------------------------------------------
        static inline pattern *ign_build(within *w)
        {
            auto_ptr<pattern> guard(w);
            first_bytes       fc;
            {
                const int         lo = w->lower;
                const int         up = w->upper;
                for(int ch=lo;ch<=up;++ch)
                {
                    fc << new domain(ch);
                    if(islower(ch)) fc << new domain(toupper(ch));
                    if(isupper(ch)) fc << new domain(tolower(ch));
                }
            }
            auto_ptr<logical> p = new op_or();
            fc.build(*p);     // optimized!
            return p.yield();
        }

        //----------------------------------------------------------------------
        //
        // duplicate a logical pattern with all ignored case operands
        //
        //----------------------------------------------------------------------
        template <typename OP>
        static inline pattern *ign_dup(OP *p)
        {
            auto_ptr<pattern> old(p);
            auto_ptr<logical> res = new OP();
            while(p->size)
                res->push_back( pattern::ignore_case(p->pop_front()) );
            return pattern::optimize(res.yield());
        }


        pattern * pattern:: ignore_case(pattern *p)
        {
            switch(p->uuid)
            {
                case single::  mark: return ign_build(p->as<single>());
                case within::  mark: return ign_build(p->as<within>());
                case except::  mark: return ign_build(p->as<except>());
                case op_and::  mark: return ign_dup<op_and>(  p->as<op_and>()  );
                case op_or::   mark: return ign_dup<op_or>(   p->as<op_or>()   );
                case op_none:: mark: return ign_dup<op_none>( p->as<op_none>() );
                default:
                    break;
            }

            return p;

        }
    }

}

