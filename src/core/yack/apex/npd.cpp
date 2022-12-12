#include "yack/apex/npd.hpp"
#include "yack/apex/primes.hpp"


namespace yack
{
    namespace apex
    {

        std::ostream & operator<<(std::ostream &os, const pnode &self)
        {
            os << *(self.p);
            const size_t n = self.n;
            if(n>1)
            {
                os << '^' << n;
            }
            return os;
        }

        pnode::~pnode() throw()
        {
            coerce(p) = 0;
            coerce(n) = 0;
        }

        pnode:: pnode(const pnode &other) throw() :
        object(),
        p(other.p),
        n(other.n),
        next(0),
        prev(0)
        {
            assert(n>0);
        }

        pnode:: pnode(const natural &p_, const size_t n_) throw() :
        object(),
        p( &p_ ),
        n(  n_ ),
        next(0),
        prev(0)
        {
            assert(n>0);
        }

        template <typename T> static inline
        void create_plist(cxx_list_of<pnode> &plist,
                          const T            &source)
        {
            assert(plist.size<=0);
            static primes &prm   = primes::instance();
            natural        value = source;

            if(value<=0) return;
            try {

                if(1==value) plist.push_back( new pnode(prm._1,1) );

                // TODO: check max prime...

                assert(value>0);
                for(const prime_knot *knot = prm->head; knot; knot=knot->next )
                {
                    const natural &den = *knot;
                    size_t         n   = 0;
                LOOK_UP:
                    {
                        natural rem  = prm._0;
                        natural quot = natural::quot(value,den,rem);
                        if(rem==0)
                        {
                            ++n;
                            value.xch(quot);
                            goto LOOK_UP;
                        }
                    }
                    if(n>0)
                    {
                        plist.push_back( new  pnode(*knot,n) );
                    }
                }

            }
            catch(...)
            {
                plist.release();
                throw;
            }
        }


        npd:: ~npd() throw()
        {
        }

        npd:: npd() throw() : plist()
        {

        }

        npd:: npd(const npd &other) : plist(other.plist)
        {
        }

        npd:: npd(const natural &n) : plist()
        {
            create_plist(plist,n);
        }

        npd:: npd(const uint_type n) : plist()
        {
            create_plist(plist,n);
        }


        npd & npd:: operator=(const npd &other)
        {
            npd temp(other);
            plist.swap_with(temp.plist);
            return *this;
        }


        std::ostream & operator<<(std::ostream &os, const npd &self)
        {
            const list_of<pnode> &plist = self.plist;
            if(plist.size<=0)
            {
                os << '0';
            }
            else
            {
                const pnode *node = plist.head;
                os << *node;
                for(node=node->next;node;node=node->next)
                {
                    os << '*' << *node;
                }
            }
            return os;
        }

    }

}

