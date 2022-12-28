#include "yack/chem/balancing/boundary.hpp"

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

        bool boundaries:: is_consistent() const throw()
        {
            const size_t n = bnd.size();
            for(size_t i=1;i<n;++i)
            {
                const boundary &lo = bnd[i];
                const boundary &hi = bnd[i+1];
                if(lo.size<=0)   return false;
                if(lo.xi>=hi.xi) return false;

            }
            return true;
        }

        static inline void bmake(boundary &b, const double x, const species &s)
        {
            coerce(b.xi) = x;
            b << &s;
        }

        void boundaries:: ins0(const double x, const species &s)
        {
            //------------------------------------------------------------------
            // create first boundary
            //------------------------------------------------------------------
            assert(0==size());
            bmake(bnd.add<const sp_fund&>(bio),x,s);
            assert(1==size());
        }


        void boundaries::ins1(const double x, const species &s)
        {
            assert(1==size());
            //------------------------------------------------------------------
            // fast second insertion
            //------------------------------------------------------------------
            boundary &b = bnd[1];
            switch( __sign::of(x,b.xi) )
            {
                case __zero__: // multiple value
                    b << &s;
                    assert(is_consistent());
                    return;

                case negative: // push front
                    assert(x<b.xi);
                    bmake(bnd.pre<const sp_fund&>(bio),x,s);
                    assert(is_consistent());
                    return;

                case positive: // push back
                    assert(x>b.xi);
                    bmake(bnd.add<const sp_fund&>(bio),x,s);
                    assert(is_consistent());
                    return;
            }
        }

        void boundaries::insN(const double x, const species &s)
        {
            //------------------------------------------------------------------
            //
            // generic insertion
            //
            //------------------------------------------------------------------
            assert(size()>=2);

            //------------------------------------------------------------------
            // check before first
            //------------------------------------------------------------------
            size_t jlo = 1;
            {
                boundary &b = bnd[jlo];
                switch( __sign::of(x,b.xi) )
                {
                    case __zero__: // multiple first value
                        b << &s;
                        assert(is_consistent());
                        return;
                        
                    case negative: // push front lower value
                        assert(x<b.xi);
                        bmake(bnd.pre<const sp_fund&>(bio),x,s);
                        assert(is_consistent());
                        return;

                    case positive: // continue
                        assert(x>b.xi);
                        break;
                }
            }

            //------------------------------------------------------------------
            // check after last
            //------------------------------------------------------------------
            const size_t jup = bnd.size(); assert(jup>jlo);
            {
                boundary &b = bnd[jup];
                switch( __sign::of(x,b.xi) )
                {
                    case __zero__: // multiple last value
                        b << &s;
                        assert(is_consistent());
                        return;

                    case negative: // continue algorithm
                        assert(x<b.xi);
                        break;

                    case positive: // push back greater value
                        assert(x>b.xi);
                        bmake(bnd.add<const sp_fund&>(bio),x,s);
                        assert(is_consistent());
                        return;
                }
            }

            //------------------------------------------------------------------
            //
            // look for insertion position
            //
            //------------------------------------------------------------------
            for(size_t jnx=jlo+1;jnx!=jup;++jnx)
            {
                assert(x>bnd[jlo].xi);
                boundary &b = bnd[jnx];
                switch( __sign::of(x,b.xi) )
                {
                    case __zero__: // multiple same value
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

            //------------------------------------------------------------------
            // insert new value
            //------------------------------------------------------------------
            bmake(bnd.ins<const sp_fund&>(bio,jlo+1),x,s);
            assert(is_consistent());
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


        static inline
        void __joined(frontier       &vanish,
                      const frontier &winner)
        {
            vanish.join(winner);
            coerce(vanish.xi) = winner.xi;
        }

        void boundaries:: analyze(frontier        &zero,
                                  const frontier &mark) const
        {

            assert(size()>0);
            assert(bnd[1].xi>0);
            assert(is_consistent());

            assert(mark.size>0);
            assert(mark.xi>=0);

            zero.release();

            //------------------------------------------------------------------
            //
            // test against head
            //
            //------------------------------------------------------------------
            {
                const boundary &self = bnd[1];
                switch( __sign::of(mark.xi,self.xi) )
                {

                    case negative: assert(mark.xi<self.xi);
                        //------------------------------------------------------
                        // mark is reached first
                        //------------------------------------------------------
                        return __joined(zero,mark);


                    case __zero__:
                        //------------------------------------------------------
                        // numerical same value
                        //------------------------------------------------------
                        zero.join(self);
                        return __joined(zero,mark);

                    case positive: assert(mark.xi>self.xi);
                        //------------------------------------------------------
                        // greater than first value
                        //------------------------------------------------------
                        if(size()<=1) {
                            return __joined(zero,self); // early return
                        }
                        break;
                }
            }


            assert(size()>1);
            assert(mark.xi>bnd[1].xi);

            //------------------------------------------------------------------
            //
            // test against tail
            //
            //------------------------------------------------------------------
            const size_t jup = bnd.size(); assert(jup>=2);
            {
                const boundary &self = bnd[jup];
                switch( __sign::of(mark.xi,self.xi) )
                {
                    case positive: assert(mark.xi>self.xi);
                        //------------------------------------------------------
                        // (highest) tail value is reached
                        //------------------------------------------------------
                        return __joined(zero,self);

                    case __zero__:
                        //------------------------------------------------------
                        // numerical same value
                        //------------------------------------------------------
                        zero.join(self);
                        return __joined(zero,mark);

                    case negative: assert(mark.xi<self.xi);
                        //------------------------------------------------------
                        // will zero the closest small boundary
                        //------------------------------------------------------
                        break;
                }
            }

            //------------------------------------------------------------------
            //
            // look up for smallest boundary below mark
            //
            //------------------------------------------------------------------
            size_t jlo = 1;
            assert(mark.xi>bnd[jlo].xi);
            assert(mark.xi<bnd[jup].xi);

            for(size_t jnx=jlo+1;jnx!=jup;++jnx)
            {
                const boundary &self = bnd[jnx];
                switch(__sign::of(mark.xi,self.xi))
                {
                    case __zero__:
                        //------------------------------------------------------
                        // numerical same value @jnx
                        //------------------------------------------------------
                        zero.join(self);
                        return __joined(zero,mark);

                    case negative:
                        //------------------------------------------------------
                        // found jlo: double break out of the loop
                        //------------------------------------------------------
                        assert(mark.xi<self.xi);
                        break;

                    case positive:
                        //------------------------------------------------------
                        // need to go on
                        //------------------------------------------------------
                        jlo = jnx;
                        continue;

                }

                break;
            }

            //------------------------------------------------------------------
            //
            // done!
            //
            //------------------------------------------------------------------
            assert(mark.xi>bnd[jlo].xi);
            return __joined(zero,bnd[jlo]);

        }

        

    }

}

