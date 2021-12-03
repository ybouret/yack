
#include "yack/jive/pattern/first-bytes.hpp"
#include "yack/signs.hpp"
#include "yack/ios/ascii/hybrid.hpp"
#include "yack/type/utils.hpp"

#include <iostream>

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

        domain:: domain(const domain &other) throw() :
        object(),
        next(0),
        prev(0),
        lower(other.lower),
        upper(other.upper)
        {
        }
        

        bool domain::owns(const uint8_t value) const throw()
        {
            return lower<=value && value<=upper;
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

        const list_of<domain> & first_bytes:: operator*() const throw()
        {
            return self;
        }


        std::ostream & operator<<(std::ostream &os, const first_bytes &p)
        {
            os << '{';
            const domain *dom = p.self.head;
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



        bool first_bytes:: is_valid() const throw()
        {
            for(const domain *dom=self.head;dom;dom=dom->next)
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

        void first_bytes:: kill() throw()
        {
            self.release();
        }

        void first_bytes:: full()
        {
            if(self.size<=0)
            {
                self.push_back( new domain(0x00,0xff) );
            }
            else
            {
                while(self.size>1)
                {
                    delete self.pop_back();
                }
                assert(1==self.size);
                coerce(self.head->lower) = 0x00;
                coerce(self.head->upper) = 0xff;
            }
        }

        void first_bytes:: grow(domain *dom) throw()
        {
            assert(dom);
            if(self.size<=0)
            {
                //--------------------------------------------------------------
                //
                // first item
                //
                //--------------------------------------------------------------
                self.push_back(dom); assert(is_valid());
            }
            else
            {
                if( (dom->upper) <= (self.head->lower) )
                {
                    //----------------------------------------------------------
                    //
                    // merge with head of push_front
                    //
                    //----------------------------------------------------------
                    switch( (self.head->lower) - (dom->upper) )
                    {
                        case 0:
                        case 1:  coerce(self.head->lower) = dom->lower; delete dom; assert(is_valid()); break;
                        default: self.push_front(dom); assert(is_valid()); break;
                    }
                }
                else
                {
                    if((self.tail->upper) <= (dom->lower))
                    {
                        //------------------------------------------------------
                        //
                        // merge with tail of push_back
                        //
                        //------------------------------------------------------
                        switch((dom->lower) - (self.tail->upper))
                        {
                            case 0:
                            case 1:  coerce(self.tail->upper) = dom->upper; delete dom; assert(is_valid()); break;
                            default: self.push_back(dom); assert(is_valid()); break;
                        }
                    }
                    else
                    {
                        //------------------------------------------------------
                        // spare prolog
                        //------------------------------------------------------
                        domains prolog;
                        while( self.size && domain::before == domain::compare(self.head,dom) )
                        {
                            prolog.push_back( self.pop_front() );
                        }

                        //------------------------------------------------------
                        // spare epilog
                        //------------------------------------------------------
                        domains epilog;
                        while(self.size && domain::after == domain::compare(self.tail,dom) )
                        {
                            epilog.push_front( self.pop_back() );
                        }

                        //------------------------------------------------------
                        // merge
                        //------------------------------------------------------
                        if(self.size)
                        {
                            coerce(dom->lower) = min_of(dom->lower,self.head->lower);
                            coerce(dom->upper) = max_of(dom->upper,self.tail->upper);
                            self.release();
                        }

                        self.swap_with(prolog);
                        self.push_back(dom);
                        self.merge_back(epilog);
                        assert(is_valid());
                    }
                }

            }
        }

        void first_bytes:: add(const uint8_t value)
        {
            grow( new domain(value) );
        }

        void first_bytes:: add(const uint8_t lo, const uint8_t up)
        {
            grow( new domain(lo,up) );
        }


        void first_bytes:: grow(list_of<domain> &doms) throw()
        {
            while(doms.size) grow(doms.pop_back());
        }

        void first_bytes:: sub(const uint8_t value)
        {
            domain *dom = self.head;
            while(dom && !(dom->owns(value))) dom=dom->next;
            if(dom)
            {
                // owned by this domain
                if(dom->lower>=dom->upper)
                {
                    // singulet
                    delete self.pop(dom);
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
                            self.insert_after(dom,nxt);
                        }
                    }
                }
            }
        }

        void first_bytes:: merge(first_bytes &other) throw()
        {
            while(other.self.size)
            {
                grow(self.pop_back());
            }
        }

        void first_bytes:: exclude(first_bytes &other)
        {
            while(other.self.size)
            {
                const size_t lower = other.self.head->lower;
                const size_t upper = other.self.head->upper;
                for(size_t i=lower;i<=upper;++i)
                {
                    sub( uint8_t(i) );
                }
                delete other.self.pop_front();
            }
        }


        first_bytes:: ~first_bytes() throw()
        {
        }

        first_bytes:: first_bytes() throw() : self()
        {
        }

        first_bytes:: first_bytes(const first_bytes &other) : self(other.self)
        {
        }
        
        
    }

}

