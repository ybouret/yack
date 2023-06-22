
#include "yack/chem/eqs/aftermath.hpp"
#include "yack/system/imported.hpp"
#include "yack/math/keto.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace Chemical
    {

        Aftermath:: ~Aftermath() noexcept
        {
        }

        Aftermath:: Aftermath() noexcept :
        extent(0),
        status(Equilibrium::Blocked),
        action(0)
        {
        }
        
        Aftermath:: Aftermath(const Extended::Real args) noexcept :
        extent(args),
        status(Equilibrium::Running),
        action(0)
        {
        }

        Aftermath:: Aftermath(const Extended::Real args,
                              const Extended::Real aerr) noexcept :
        extent(args),
        status(Equilibrium::Running),
        action(aerr)
        {
        }

        Aftermath:: Aftermath(const Aftermath &am) noexcept :
        extent(am.status),
        status(am.status),
        action(am.action)
        {

        }

        Aftermath & Aftermath:: operator=(const Aftermath &am) noexcept
        {
            coerce(extent) = am.extent;
            coerce(status) = am.status;
            coerce(action) = am.action;
            return *this;
        }


        std::ostream & operator<<(std::ostream &os, const Aftermath &am)
        {
            os << Equilibrium::StatusText(am.status) << " @" << std::setw(15) << *am.extent << " (action=" << *am.action << ")";
            return os;
        }


        Extended::Real Aftermath:: makeExtent(const Components               &eq,
                                              const readable<Extended::Real> &Corg,
                                              const readable<Extended::Real> &Cend,
                                              const IndexLevel                level,
                                              Extended::Add                  &xadd)
        {
            xadd.free();
            assert(eq->size>0);
            const Extended::Real den = eq->size;
            for(const cNode *node=eq->head;node;node=node->next)
            {
                const Component &cc = ***node;
                const Species   &sp = *cc;
                const size_t     sj = sp.indx[level];
                const Extended::Real del = (Cend[sj]-Corg[sj])/cc.xn;
                xadd.append(del);
            }
            return xadd.reduce()/den;
        }

        Aftermath Aftermath:: Evaluate(const Equilibrium              &eq,
                                       const Extended::Real           &K,
                                       const Extended::Real           &S,
                                       writable<Extended::Real>       &Cend,
                                       const readable<Extended::Real> &Corg,
                                       Extents                        &extents,
                                       const IndexLevel                level,
                                       Extended::Mul                  &xmul,
                                       Extended::Add                  &xadd,
                                       writable<Extended::Real>       &Ctmp)
        {
            const Extended::Real _0 =  0;
            const Extended::Real m1 = -1;
            Extended::Triplet    xi = { _0, _0, _0 };
            Extended::Triplet    ma = { _0, _0, _0 };

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            assert(Cend.size()>=Corg.size());
            assert(Ctmp.size()>=Corg.size());

            for(size_t i=Corg.size();i>0;--i)
            {
                Cend[i] = Ctmp[i] = Corg[i];
            }
            Extended::Real absXi = 0;       // must decrease
            bool           moved = false;   // true but not the first cycle

            //------------------------------------------------------------------
            //
            //
            // Lookup status.
            //
            //
            //------------------------------------------------------------------
        LOOKUP:
            switch( extents.build(eq,Cend,level) )
            {
                    //----------------------------------------------------------
                    //
                    // sanity check
                    //
                    //----------------------------------------------------------
                case LimitedByNone: throw imported::exception(eq.name(),"empty equilibrium");


                case LimitedByProd:
                    //----------------------------------------------------------
                    //
                    // ENTER: limited by product(s) only
                    //
                    //----------------------------------------------------------
                    xi.b = _0;
                    ma.b = eq.massAction(xmul,K,Cend,level);
                    switch( __sign::of(ma.b.m) )
                    {
                        case __zero__:
                            //--------------------------------------------------
                            // Cend is exact numerical solution
                            //--------------------------------------------------
                            return moved ? Aftermath(makeExtent(eq,Corg,Cend,level,xadd)) : Aftermath(_0);

                        case negative:
                            //--------------------------------------------------
                            // move prod to get a positive mass action = K
                            //--------------------------------------------------
                            assert(extents.prod>0);
                            xi.a = -extents.prod;
                            eq.make(Ctmp, level, Cend, level, xi.a);
                            extents.prod.nullify(Ctmp,level);
                            ma.a = K;
                            xi.c = xi.b;
                            ma.c = ma.b;
                            break;

                        case positive:
                            //--------------------------------------------------
                            // increase concentrations with scaling
                            //--------------------------------------------------
                            assert(S>0);
                            xi.a = xi.b;
                            ma.a = ma.b;
                            xi.c = S;
                        FWD:
                            eq.make(Ctmp,level,Cend,level,xi.c);
                            ma.c = eq.massAction(xmul,K,Ctmp,level);
                            switch( __sign::of(ma.c.m) )
                            {
                                case __zero__:
                                    keto::save(Cend,Ctmp);
                                    return Aftermath(makeExtent(eq,Corg,Cend,level,xadd));
                                case positive: xi.c += S; goto FWD;
                                case negative: break;
                            }
                            assert(ma.c<0);
                            break;
                    }
                    assert(xi.a<xi.c);
                    assert(ma.a>0);
                    assert(ma.c<0);
                    //----------------------------------------------------------
                    //
                    // LEAVE: limited by product(s) only
                    //
                    //----------------------------------------------------------
                    break;


                case LimitedByReac:
                    //----------------------------------------------------------
                    //
                    // ENTER: limited by reactant(s) only
                    //
                    //----------------------------------------------------------
                    xi.b = _0;
                    ma.b = eq.massAction(xmul,K,Cend,level);
                    
                    switch( __sign::of(ma.b.m) )
                    {
                        case __zero__:
                            //--------------------------------------------------
                            // Cend is exact numerical solution
                            //--------------------------------------------------
                            return moved ? Aftermath(makeExtent(eq,Corg,Cend,level,xadd)) : Aftermath(_0);

                        case positive:
                            //--------------------------------------------------
                            // move reac to get a negative action = -1
                            //--------------------------------------------------
                            xi.c =  extents.reac;
                            eq.make(Ctmp, level, Cend, level, xi.c);
                            extents.reac.nullify(Ctmp,level);
                            ma.c = m1;
                            xi.a = xi.b;
                            ma.a = ma.b;
                            break;

                        case negative:
                            //--------------------------------------------------
                            // make reac up to positive mass action
                            //--------------------------------------------------
                            xi.c = xi.b;
                            ma.c = ma.b;
                            xi.a = -S;
                        REV:
                            eq.make(Ctmp,level,Cend,level,xi.a);
                            ma.a = eq.massAction(xmul,K,Ctmp,level);
                            switch( __sign::of(ma.a.m) )
                            {
                                case __zero__:
                                    keto::save(Cend,Ctmp);
                                    return  Aftermath(makeExtent(eq,Corg,Cend,level,xadd));
                                case negative: xi.a -= S; goto REV;
                                case positive: break;
                            }
                            assert(ma.a>0);
                            break;
                    }

                    assert(xi.a<xi.c);
                    assert(ma.a>0);
                    assert(ma.c<0);
                    //----------------------------------------------------------
                    //
                    // LEAVE: limited by reactant(s) only
                    //
                    //----------------------------------------------------------
                    break;



                case LimitedByBoth:
                    //----------------------------------------------------------
                    //
                    // ENTER: limited by reactant(s) and product(s)
                    //
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    // check blocked or not, can only happen at first passage
                    //----------------------------------------------------------
                    if(extents.reac.isBlocking() && extents.prod.isBlocking())
                    {
                        assert(!moved);
                        return Aftermath();
                    }

                    //----------------------------------------------------------
                    // check if xi=0 is solution for Cend, otherwise init
                    //----------------------------------------------------------
                    xi.b = _0;
                    ma.b = eq.massAction(xmul,K,Cend,level);
                    switch( __sign::of(ma.b.m) )
                    {


                        case __zero__:
                            //--------------------------------------------------
                            // found exact solution
                            //--------------------------------------------------
                            return moved ? Aftermath(makeExtent(eq,Corg,Cend,level,xadd)) : Aftermath(_0);

                        case negative:
                            //--------------------------------------------------
                            // move prod to get a positive action from reac only
                            //--------------------------------------------------
                            assert(extents.prod>0);
                            xi.a = -extents.prod;
                            eq.make(Ctmp, level, Cend, level, xi.a);
                            extents.prod.nullify(Ctmp,level);
                            ma.a = eq.reacMassAction(xmul,K,Ctmp,level); assert(ma.a>0);
                            xi.c = xi.b;
                            ma.c = ma.b;
                            break;

                        case positive:
                            //--------------------------------------------------
                            // move reac to get a negative action from prod only
                            //--------------------------------------------------
                            assert(extents.reac>0);
                            xi.c =  extents.reac;
                            eq.make(Ctmp, level, Cend, level, xi.c);
                            extents.reac.nullify(Ctmp,level);
                            ma.c = -eq.prodMassAction(xmul,Ctmp,level);
                            xi.a = xi.b;
                            ma.a = ma.b;
                    }

                    assert(xi.a<xi.c);
                    assert(ma.a>0);
                    assert(ma.c<0);
                    //----------------------------------------------------------
                    //
                    // LEAVE: limited by reactant(s) and product(s)
                    //
                    //----------------------------------------------------------
                    break;

            }

            //------------------------------------------------------------------
            //
            //
            // bisection from current setup
            //
            //
            //------------------------------------------------------------------
            assert(xi.a<xi.c);
            assert(ma.a>0);
            assert(ma.c<0);
            Extended::Real width = xi.c - xi.a;
            while(true)
            {
                xi.b = (xi.a+xi.c).half();
                eq.make(Ctmp, level, Cend, level, xi.b);
                ma.b = eq.massAction(xmul,K,Ctmp,level);
                switch( __sign::of(ma.b) )
                {
                    case __zero__:
                        keto::save(Cend,Ctmp);
                        xi.b = makeExtent(eq,Corg,Cend,level,xadd);
                        return Aftermath( xi.b );

                    case negative:
                        xi.c = xi.b;
                        ma.c = ma.b;
                        break;

                    case positive:
                        xi.a = xi.b;
                        ma.a = ma.b;
                        break;
                }
                const Extended::Real newWidth = xi.c - xi.a;
                if(newWidth>=width)
                {
                    break;
                }
                width = newWidth;
            }

            const Extended::Real absTmp = xi.b.abs();
            keto::save(Cend,Ctmp);
            if(moved && absTmp>=absXi)
            {
                // numerical zero
                xi.b = makeExtent(eq,Corg,Cend,level,xadd);
                return Aftermath(xi.b,ma.b);
            }

            absXi = absTmp;
            moved = true;
            goto LOOKUP;
        }
    }

}

