

#include "yack/chem/eqs/composite.hpp"
#include "yack/apex.hpp"
#include "yack/type/abs.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
    {

        composite:: coeff:: coeff() throw() : lhs(0), rhs(0) {}


        void composite:: coeff:: normalize() throw()
        {
            if(lhs<0 && rhs<0) { lhs=-lhs;rhs=-rhs; }
            if(rhs<lhs) cswap(lhs,rhs);
            assert(rhs>0);
        }

        size_t composite:: collect(coeffs           &cof,
                                   const components &lhs,
                                   const components &rhs)
        {

            assert(&lhs!=&rhs);
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            cof.free();

            //------------------------------------------------------------------
            //
            // loop over LHS
            //
            //------------------------------------------------------------------
            for(const cnode *lnode=lhs.head();lnode;lnode=lnode->next)
            {
                //--------------------------------------------------------------
                // LHS info
                //--------------------------------------------------------------
                const component &lcomp  = ***lnode; // component
                const size_t     lindx  =  **lcomp; // species index
                const int        lcoef  = lcomp.nu; // species coeff

                //--------------------------------------------------------------
                //
                // loop over RHS
                //
                //--------------------------------------------------------------
                for(const cnode *rnode=rhs.head();rnode;rnode=rnode->next)
                {
                    //----------------------------------------------------------
                    // RHS info
                    //----------------------------------------------------------
                    const component &rcomp  = ***rnode; // component
                    const size_t     rindx  =  **rcomp; // species index

                    //----------------------------------------------------------
                    // not the same
                    //----------------------------------------------------------
                    if(rindx!=lindx) continue;

                    //----------------------------------------------------------
                    // guess coefficient
                    //----------------------------------------------------------
                    coeff guess(lcoef,rcomp.nu);

                    //----------------------------------------------------------
                    // normalize
                    //----------------------------------------------------------
                    guess.normalize(); assert(guess.rhs>0);

                    //----------------------------------------------------------
                    // check existing
                    //----------------------------------------------------------
                    bool  found = false;
                    for(size_t i=cof.size();i>0;--i)
                    {
                        if(guess == cof[i]) { found=true; break; }
                    }

                    //----------------------------------------------------------
                    // if new, append to coefficients
                    //----------------------------------------------------------
                    if(!found)
                    {
                        cof << guess;
                    }

                }

            }

            //------------------------------------------------------------------
            //
            // transform coefficients to weights
            //
            //------------------------------------------------------------------
            for(size_t i = cof.size();i>0;--i)
            {
                coeff     &c = cof[i]; assert(c.rhs>0); assert(c.lhs!=0);
                const apq  q(absolute(c.rhs),absolute(c.lhs));
                coeff      w( q.num.cast_to<int>("lhs weight"), q.den.cast_to<int>("rhs weight"));
                if(c.lhs>0) w.lhs = -w.lhs;
                if(0 != w.lhs*c.lhs+w.rhs*c.rhs)
                    throw imported::exception("chemical::composite", "failure to compose components!!");
                c.lhs = w.lhs; assert(c.lhs!=0);
                c.rhs = w.rhs; assert(c.rhs>0);
            }

            //------------------------------------------------------------------
            //
            // all done
            //
            //------------------------------------------------------------------
            return cof.size();
        }

    }

}

#include "yack/chem/library.hpp"
#include <iomanip>

 
namespace yack
{
    namespace chemical
    {

        static inline void add_sto(writable<int>    &sto,
                                   const components &cmp,
                                   const int         fac) throw()
        {
            for(const cnode *node=cmp.head();node;node=node->next)
            {
                const component &c = ***node;
                const species   &s = *c;
                const size_t     j = *s;
                sto[j] += fac * c.nu;
            }
        }

        static inline void new_sto(writable<int>          &sto,
                                   const components       &lhs,
                                   const components       &rhs,
                                   const composite::coeff &c) throw()
        {
            sto.ld(0);
            add_sto(sto,lhs,c.lhs);
            add_sto(sto,rhs,c.rhs);
        }

        static inline string composite_name_(const int a, const string &A, const int b, const string &B)
        {
            assert(a);
            assert(b);
            string res;
            switch(a)
            {
                case -1: res += '-'; break;
                case  1: break;
                default:
                    assert( absolute(a) > 1 );
                    res += vformat("%d*",a);
            }
            res += A;

            switch(b)
            {
                case -1: res += '-'; break;
                case  1: res += '+'; break;
                default:
                    assert( absolute(b) > 1 );
                    res += vformat("%+d*",b);
            }
            res += B;

            return res;
        }


        static inline string composite_name(const composite::coeff &c,
                                            const equilibrium      &lhs,
                                            const equilibrium      &rhs)
        {
            const int     l = c.lhs;
            const string &L = lhs.name;
            const int     r = c.rhs;
            const string &R = rhs.name;

            switch( __sign::pair( __sign::of(l), __sign::of(r) ) )
            {
                case pp_pair:
                case nn_pair:
                case pn_pair:
                    return composite_name_(l,L,r,R);

                case np_pair:
                    return composite_name_(r,R,l,L);


                default:
                    break;

            }

            throw imported::exception("chemical::composite_name","invalid coefficients (%d,%d)",l,r);

        }

        
        static inline void  composite_fill( equilibrium &eq, const readable<int> &sto, const library &lib )
        {
            const size_t M = sto.size();
            for(size_t i=1;i<=M;++i)
            {
                const int nu = sto[i];
                if(nu) eq( lib[i], nu );
            }
        }


        double composite:: getK(const coeff &c, const double lhsK, const double rhsK, rmulops &xmul)
        {
            xmul.free();
            xmul.ipower(lhsK,c.lhs);
            xmul.ipower(rhsK,c.rhs);
            return xmul.query();
        }


        namespace
        {
            class composite_equilibrium : public equilibrium
            {
            public:
                inline virtual ~composite_equilibrium() throw() {}

                inline explicit composite_equilibrium(const string           &uid,
                                                      const size_t            idx,
                                                      const composite::coeff &c,
                                                      const double           &l,
                                                      const double           &r,
                                                      rmulops                &xm) :
                equilibrium(uid,idx),
                power(c),
                lhs_K(l),
                rhs_K(r),
                xmul(xm)
                {
                }

                const composite::coeff power;
                const double          &lhs_K;
                const double          &rhs_K;
                rmulops               &xmul;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(composite_equilibrium);

                virtual double getK(double) const
                {
                    return composite::getK(power,lhs_K,rhs_K,xmul);
                }
            };

        }


        void composite:: scatter(equilibria             &couples,
                                 library                &libcopy,
                                 const equilibria       &singles,
                                 const readable<double> &K,
                                 rmulops                &xmul)
        {

            const size_t M = libcopy.size();
            coeffs       cof(M,as_capacity);
            vector<int>  sto(M,0);
            size_t       idx = singles.size();

            for(const enode *lhs=singles.head();lhs;lhs=lhs->next)
            {
                const equilibrium &LHS = ***lhs;
                for(const enode *rhs=lhs->next;rhs;rhs=rhs->next)
                {
                    const equilibrium &RHS = ***rhs;
                    const size_t       nc  = collect(cof,LHS,RHS);
                    if(nc>0)
                    {
                        for(size_t i=1;i<=nc;++i)
                        {
                            coeff &c = cof[i]; assert(c.rhs>0);
                            std::cerr <<    "(" << std::setw(3) << c.lhs << ")*"; singles.pad(std::cerr << LHS.name,LHS);
                            std::cerr << " + (" << std::setw(3) << c.rhs << ")*"; singles.pad( std::cerr <<RHS.name,RHS);
                            std::cerr << std::endl;

                            const double &lhsK    = K[*LHS];
                            const double &rhsK    = K[*RHS];
                            const double  forwardK = getK(c,lhsK,rhsK,xmul);
                            //std::cerr << "forwardK = " << forwardK << std::endl;
                            if(forwardK<1)
                            {
                                c.lhs = -c.lhs;
                                c.rhs = -c.rhs;
                            }
                            //std::cerr << "create with " << c << std::endl;
                            const string uid = composite_name(c,LHS,RHS);
                            //std::cerr << "uid=<"<< uid << ">" << std::endl;
                            equilibrium &eq  = couples.use( new composite_equilibrium(uid,++idx,c,lhsK,rhsK,xmul) );
                            new_sto(sto,LHS,RHS,c);
                            composite_fill(eq,sto,libcopy);

                        }
                    }
                }

            }
            std::cerr << "couples=" << couples << std::endl;
        }

    }

}
