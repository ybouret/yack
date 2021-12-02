#include "yack/jive/pattern/partition.hpp"
#include "yack/signs.hpp"
#include <iostream>
#include "yack/ios/ascii/hybrid.hpp"
#include "yack/type/utils.hpp"

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
        next(0),
        prev(0),
        lower(v),
        upper(v)
        {
        }

        domain:: domain(const uint8_t a, const uint8_t b) throw():
        object(),
        next(0),
        prev(0),
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

        domain::position domain::compare(const domain *lhs, const domain *rhs) throw()
        {
            assert(NULL!=lhs);
            assert(NULL!=rhs);
            if(lhs->upper <= rhs->lower)
            {
                switch(rhs->lower - lhs->upper)
                {
                    case 0:
                    case 1:
                        return overlap;
                    default:
                        break;
                }
                return before;
            }
            else
            {
                assert(lhs->upper>rhs->lower);
                if(rhs->upper <= lhs->lower)
                {
                    switch(lhs->lower-rhs->upper)
                    {
                        case 0:
                        case 1:
                            return overlap;
                        default:
                            break;
                    }
                    return after;
                }
                else
                {
                    assert(lhs->upper>rhs->lower);
                    assert(rhs->upper>lhs->lower);
                    return overlap;
                }
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
                os << '-';
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


        bool partition:: is_valid() const throw()
        {
            for(const domain *dom=head;dom;dom=dom->next)
            {
                if(dom->lower>dom->upper)
                {
                    std::cerr << "invalid domain " << *dom << std::endl;
                    return false;
                }

                if(dom->next)
                {
                    if( (dom->next->lower) <= (dom->upper))
                    {
                        std::cerr << "overlapping level-1 " << *dom << " " << *(dom->next) << std::endl;
                        return false;
                    }

                    if(dom->next->lower-dom->upper<=1)
                    {
                        std::cerr << "overlapping level-2 " << *dom << " " << *(dom->next) << std::endl;
                        return false;
                    }
                }
            }

            return true;
        }


        void partition:: grow(domain *dom) throw()
        {
            assert(dom);
            if(size<=0)
            {
                //--------------------------------------------------------------
                //
                // first item
                //
                //--------------------------------------------------------------
                push_back(dom); assert(is_valid());
            }
            else
            {
                if( (dom->upper) <= (head->lower) )
                {
                    //----------------------------------------------------------
                    //
                    // merge with head of push_front
                    //
                    //----------------------------------------------------------
                    switch( (head->lower) - (dom->upper) )
                    {
                        case 0:
                        case 1:  coerce(head->lower) = dom->lower; delete dom; assert(is_valid()); break;
                        default: push_front(dom); assert(is_valid()); break;
                    }
                }
                else
                {
                    if((tail->upper) <= (dom->lower))
                    {
                        //------------------------------------------------------
                        //
                        // merge with tail of push_back
                        //
                        //------------------------------------------------------
                        switch((dom->lower) - (tail->upper))
                        {
                            case 0:
                            case 1:  coerce(tail->upper) = dom->upper; delete dom; assert(is_valid()); break;
                            default: push_back(dom); assert(is_valid()); break;
                        }
                    }
                    else
                    {
                        //------------------------------------------------------
                        // spare prolog
                        //------------------------------------------------------
                        domains prolog;
                        while( size && domain::before == domain::compare(head,dom) )
                        {
                            prolog.push_back( pop_front() );
                        }

                        //------------------------------------------------------
                        // spare epilog
                        //------------------------------------------------------
                        domains epilog;
                        while(size && domain::after == domain::compare(tail,dom) )
                        {
                            epilog.push_front( pop_back() );
                        }

                        //------------------------------------------------------
                        // merge
                        //------------------------------------------------------
                        if(size)
                        {
                            coerce(dom->lower) = min_of(dom->lower,head->lower);
                            coerce(dom->upper) = max_of(dom->upper,tail->upper);
                            release();
                        }

                        swap_with(prolog);
                        push_back(dom);
                        merge_back(epilog);
                        assert(is_valid());
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


        void partition:: grow(list_of<domain> &doms) throw()
        {
            while(doms.size) grow(doms.pop_back());
        }

    }

}

