
#include "yack/jive/pattern/first-bytes.hpp"
#include "yack/signs.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include "yack/type/utils.hpp"

#include <iostream>

namespace yack
{
    namespace jive
    {

        domain:: ~domain() noexcept
        {
            coerce(lower) = 0;
            coerce(upper) = 0;
        }

        domain:: domain(const uint8_t v) noexcept :
        object(),
        next(0),
        prev(0),
        lower(v),
        upper(v)
        {
        }

        domain:: domain(const uint8_t a, const uint8_t b) noexcept:
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

        domain:: domain(const domain &other) noexcept :
        object(),
        next(0),
        prev(0),
        lower(other.lower),
        upper(other.upper)
        {
        }
        

        bool domain::owns(const uint8_t value) const noexcept
        {
            return lower<=value && value<=upper;
        }

        



        domain::position domain::compare(const domain *lhs, const domain *rhs) noexcept
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




        std::ostream & operator<<(std::ostream &os, const first_bytes &p)
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



        bool first_bytes:: is_valid() const noexcept
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

       

        void first_bytes:: all()
        {
            if(size<=0)
            {
                push_back( new domain(0x00,0xff) );
            }
            else
            {
                while(size>1)
                {
                    delete pop_back();
                }
                assert(1==size);
                coerce(head->lower) = 0x00;
                coerce(head->upper) = 0xff;
            }
        }

        first_bytes &  first_bytes:: operator<<( domain *dom ) noexcept
        {
            add(dom);
            return *this;
        }

        
        first_bytes &  first_bytes:: operator<<(list_of<domain> &doms) noexcept
        {
            while(doms.size) add(doms.pop_back());
            return *this;
        }

        void first_bytes:: add(domain *dom) noexcept
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
                            prolog.push_back(pop_front() );
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


        void first_bytes:: sub(const uint8_t value)
        {
            domain *dom = head;
            while(dom && !(dom->owns(value))) dom=dom->next;
            if(dom)
            {
                // owned by this domain
                if(dom->lower>=dom->upper)
                {
                    // singulet
                    delete pop(dom);
                }
                else
                {
                    if(value==dom->lower)
                    {
                        ++coerce(dom->lower);
                    }
                    else
                    {
                        if(value==dom->upper)
                        {
                            --coerce(dom->upper);
                        }
                        else
                        {
                            assert(dom->lower<value);
                            assert(value<dom->upper);
                            domain *nxt = new domain(value+1,dom->upper);
                            coerce(dom->upper) = value-1;
                            insert_after(dom,nxt);
                        }
                    }
                }
            }
        }

        first_bytes & first_bytes:: operator -=(const uint8_t c)
        {
            sub(c);
            return *this;
        }

        first_bytes &  first_bytes::  operator -=(list_of<domain> &other)
        {
            while(other.size)
            {
                const size_t lower = other.head->lower;
                const size_t upper = other.head->upper;
                for(size_t i=lower;i<=upper;++i)
                {
                    sub( uint8_t(i) );
                }
                delete other.pop_front();
            }
            return *this;
        }


        first_bytes & first_bytes:: inverse()
        {
            domains   content;
            swap_with(content);
            all();
            return (*this -= content);
        }

        first_bytes:: ~first_bytes() noexcept
        {
        }

        first_bytes:: first_bytes() noexcept :
        object(), counted(), domains()
        {
        }

        first_bytes:: first_bytes(const first_bytes &other) :
        object(), counted(), domains(other)
        {
        }
        
        bool first_bytes:: has(const uint8_t code) const noexcept
        {
            assert(is_valid());
            if(size)
            {
                if(code<head->lower)
                {
                    return false;
                }
                else
                {
                    if(code>tail->upper)
                    {
                        return false;
                    }
                    else
                    {
                        for(const domain *dom=head;dom;dom=dom->next)
                        {
                            if(dom->owns(code)) return true;
                        }
                        return false;
                    }
                }
            }
            else
            {
                return false;
            }


        }

    }

}

