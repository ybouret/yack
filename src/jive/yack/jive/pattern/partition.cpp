#include "yack/jive/pattern/partition.hpp"
#include "yack/signs.hpp"
#include <iostream>
#include "yack/ios/ascii/hybrid.hpp"

namespace yack
{
    namespace jive
    {
        domain:: ~domain() throw()
        {
            coerce(lower) = 0;
            coerce(upper) = 0;
        }

        domain:: domain(const uint8_t v) throw() :
        object(),
        lower(v),
        upper(v)
        {
        }

        domain:: domain(const uint8_t a, const uint8_t b) throw():
        object(),
        lower(a),
        upper(b)
        {
            switch(__sign::of(lower,upper))
            {
                case negative: assert(lower<upper); break;
                case positive: assert(lower>upper); coerce_cswap(lower,upper); break;
                case __zero__: assert(lower==upper); break;
            }
        }


        static inline void display_byte(std::ostream &os, const uint8_t b)
        {
            os << '\'' << ios::ascii::hybrid[b] << '\'';
        }

        std::ostream & operator<<(std::ostream &os, const domain &d)
        {
            if(d.lower>=d.upper)
            {
                display_byte(os,d.lower);
            }
            else
            {

                os << '[';
                display_byte(os,d.lower);
                os << ':';
                display_byte(os,d.upper);
                os << ']';
            }
            return os;
        }


        std::ostream & operator<<(std::ostream &os, const partition &p)
        {
            os << '{';
            const domain *dom = p.head;
            if(dom)
            {
                os << *dom;
                while(NULL != (dom=dom->next) )
                {
                    os << ';' << *dom;
                }
            }
            os << '}';
            return os;
        }

        partition:: partition() throw() : domains() {}

        partition:: ~partition() throw()
        {
        }


        void partition:: grow(domain *dom) throw()
        {
            assert(dom);
            if(size<=0)
            {
                push_back(dom);
            }
            else
            {

                if(dom->upper <= head->lower)
                {
                    //__________________________________________________________
                    //
                    // merge with head of push_front
                    //__________________________________________________________
                    switch((head->lower)-(dom->upper))
                    {
                        case 0:
                        case 1:
                            coerce(head->lower)=dom->lower;
                            delete dom;
                            break;
                        default:
                            push_front(dom);
                    }
                }
                else
                {
                    assert(dom->upper > head->lower);

                    if(tail->upper<=dom->lower)
                    {
                        //______________________________________________________
                        //
                        // merge with tail or push_back
                        //______________________________________________________
                        switch((dom->lower) - (tail->upper))
                        {
                            case 0:
                            case 1:
                                coerce(tail->upper) = dom->upper;
                                delete dom;
                                break;

                            default:
                                push_back(dom);
                        }
                    }
                    else
                    {

                    }
                }

            }
        }

        void partition:: add(const uint8_t value)
        {
            grow( new domain(value) );
        }

        void partition:: add(const uint8_t lo, const uint8_t up)
        {
            grow( new domain(lo,up) );
        }


    }

}

