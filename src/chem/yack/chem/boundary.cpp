#include "yack/chem/boundary.hpp"

namespace yack
{
    namespace chemical
    {

        boundary:: ~boundary() throw()
        {
        }

        boundary:: boundary(const sp_fund &io) throw() :
        frontier(io)
        {
        }

        

    }

}

namespace yack
{
    namespace chemical
    {

        boundaries:: ~boundaries() throw()
        {
        }

        boundaries:: boundaries(const size_t m, const sp_fund &io):
        readable<boundary>(),
        bnd(m),
        bio(io)
        {
        }

        size_t boundaries:: size() const throw() { return bnd.size(); }

        const boundary & boundaries:: operator[](const size_t i) const throw()
        {
            assert(i>=1);
            assert(i<=bnd.size());
            return bnd[i];
        }


        void boundaries:: destroy() throw()
        {
            bnd.free();
        }

        static inline void bmake(boundary &b, const double x, const species &s)
        {
            coerce(b.xi) = x;
            b << &s;
        }

        void boundaries:: ins0(const double x, const species &s)
        {
            assert(0==size());
            // create first boundary
            bmake(bnd.add<const sp_fund&>(bio),x,s);
        }


        void boundaries::ins1(const double x, const species &s)
        {
            assert(1==size());
            // fast second insertion
            boundary &b = bnd[1];
            switch( __sign::of(x,b.xi) )
            {
                case __zero__: // multiple value
                    b << &s;
                    return;

                case negative: // push front
                    assert(x<b.xi);
                    bmake(bnd.pre<const sp_fund&>(bio),x,s);
                    return;

                case positive: // push back
                    assert(x>b.xi);
                    bmake(bnd.add<const sp_fund&>(bio),x,s);
                    return;
            }
        }

        void boundaries::insN(const double x, const species &s)
        {
            assert(size()>=2);
            size_t jlo = 1;
            {
                boundary &b = bnd[jlo];
                switch( __sign::of(x,b.xi) )
                {
                    case __zero__: // multiple first value
                        b << &s;
                        return;
                        
                    case negative: // push front lower value
                        assert(x<b.xi);
                        bmake(bnd.add<const sp_fund&>(bio),x,s);
                        return;

                    case positive: // continue
                        assert(x>b.xi);
                        break;
                }
            }

            const size_t jup = bnd.size(); assert(jup>jlo);
            {
                boundary &b = bnd[jup];
                switch( __sign::of(x,b.xi) )
                {
                    case __zero__: // multiple last value
                        b << &s;
                        return;

                    case negative: // continue algorithm
                        assert(x<b.xi);
                        break;

                    case positive: // push back greater value
                        assert(x>b.xi);
                        bmake(bnd.add<const sp_fund&>(bio),x,s);
                        return;
                }
            }

            for(size_t jnx=jlo+1;jnx!=jup;++jnx)
            {
                assert(x>bnd[jlo].xi);
                boundary &b = bnd[jnx];
                switch( __sign::of(x,b.xi) )
                {
                    case __zero__: // mutliple same value
                        b << &s;
                        return;
                    case positive: // check next interval
                        jlo = jnx;
                        continue;

                    case negative: // done
                        break;
                }
                assert(x>bnd[jlo].xi);
                assert(x<bnd[jnx].xi);
                break;
            }
            assert(jlo<jup);
            assert(x>bnd[jlo].xi);
            assert(x<bnd[jlo+1].xi);

            
            exit(0);

        }


        void boundaries:: upgrade(const double x, const species &s)
        {
            assert(x>=0);


            switch(bnd.size())
            {
                case  0: ins0(x,s); return;
                case  1: ins1(x,s); return;
                default: insN(x,s); return;
            }

        }

    }

}

