

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

            // initialize
            cof.free();

            // loop over LHS
            for(const cnode *lnode=lhs.head();lnode;lnode=lnode->next)
            {
                const component &lcomp  = ***lnode;
                const size_t     lindx  =  **lcomp;
                const int        lcoef  = lcomp.nu;

                // loop over RHS
                for(const cnode *rnode=rhs.head();rnode;rnode=rnode->next)
                {
                    const component &rcomp  = ***rnode;
                    const size_t     rindx  =  **rcomp;

                    // not the same
                    if(rindx!=lindx) continue;

                    //std::cerr << "\t\tsharing " << (*rcomp).name << std::endl;

                    // guess coefficient
                    coeff guess(lcoef,rcomp.nu);
                    guess.normalize();

                    // check existing
                    bool  found = false;
                    for(size_t i=cof.size();i>0;--i)
                    {
                        if(guess == cof[i]) { found=true; break; }
                    }

                    if(!found)
                    {
                        cof << guess;
                    }

                }

            }

            // transform coefficients to weights
            for(size_t i = cof.size();i>0;--i)
            {
                coeff     &c = cof[i]; assert(c.rhs>0); assert(c.lhs!=0);
                const apq  q(absolute(c.rhs),absolute(c.lhs));
                coeff      w( q.num.cast_to<int>("lhs weight"), q.den.cast_to<int>("rhs weight"));
                if(c.lhs>0) w.lhs = -w.lhs;
                if(0 != w.lhs*c.lhs+w.rhs*c.rhs)
                    throw imported::exception("chemical::composite", "failure to compose components!!");
                c.lhs = w.lhs;
                c.rhs = w.rhs; assert(c.rhs>0);
            }

            // all done
            return cof.size();
        }

    }

}

#include "yack/chem/library.hpp"
#include "yack/arith/gcd.h"

namespace yack
{
    namespace chemical
    {

        static inline void building_coeff(writable<int>    &sto,
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

#if 0
        static inline void simplify_coeff(writable<int> &sto)
        {
            const size_t     M = sto.size();
            vector<uint64_t> U(M,as_capacity);
            for(size_t i=M;i>0;--i)
            {
                const int I = sto[i];
                if(I) U << absolute(I);
            }
            assert(U.size()>0);;
            uint64_t g = U[1];
            for(size_t i=U.size();i>1;--i)
            {
                g = yack_gcd64(g,U[i]);
            }
            for(size_t i=M;i>0;--i)
            {
                sto[i] /= g;
            }
        }
#endif

        static inline string composite_name(const int a, const string &A, const int b, const string &B)
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
                    res += vformat("%d",a);
            }
            res += A;

            switch(b)
            {
                case -1: res += '-'; break;
                case  1: res += '+'; break;
                default:
                    assert( absolute(b) > 1 );
                    res += vformat("%+d",b);
            }
            res += B;

            return res;
        }

        static inline equilibrium &composite_fill( equilibrium &eq, const readable<int> &sto, const library &lib )
        {
            const size_t M = sto.size();
            for(size_t i=1;i<=M;++i)
            {
                const int nu = sto[i];
                if(nu) eq( lib[i], nu );
             }
            return eq;
        }

        namespace
        {

            class mul_composite_equilibrium : public equilibrium
            {
            public:
                explicit mul_composite_equilibrium(const string &uid,
                                                   const size_t  idx,
                                                   const unsigned lhs_p,
                                                   const double  &lhs_k,
                                                   const unsigned rhs_p,
                                                   const double  &rhs_k) :
                equilibrium(uid,idx),
                lp(lhs_p),
                lK(lhs_k),
                rp(rhs_p),
                rK(rhs_k)
                {
                }

                const unsigned lp;
                const double  &lK;
                const unsigned rp;
                const double  &rK;

                mutable rmulops xmul;

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(mul_composite_equilibrium);

                virtual double getK(double) const
                {
                    xmul.free();
                    xmul.ld(lK,lp);
                    xmul.ld(rK,rp);
                    return xmul.query();
                }
            };


        }


        void composite:: scatter(equilibria             &couples,
                                 library                &libcopy,
                                 const equilibria       &singles,
                                 const readable<double> &K)
        {

            const size_t M = libcopy.size();
            coeffs       cof(M,as_capacity);
            vector<int>  sto(M,0);
            size_t idx = singles.size();
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
                            const coeff &c = cof[i];
                            std::cerr << "(" << c.lhs << ")*" << LHS.name << " + (" << c.rhs << ")*" << RHS.name << std::endl;
                            sto.ld(0);
                            building_coeff(sto,LHS,c.lhs);
                            building_coeff(sto,RHS,c.rhs);

                            if(c.lhs<0)
                            {

                            }
                            else
                            {
                                assert(c.lhs>0);
                                assert(c.rhs>0);
                                const string       uid = composite_name(c.lhs,LHS.name,c.rhs,RHS.name);
                                equilibrium       &eq  = composite_fill( couples.use( new mul_composite_equilibrium(uid,++idx,c.lhs,K[*LHS],c.rhs,K[*RHS]) ), sto, libcopy );
                                std::cerr << "\t" << eq << std::endl;
                            }


                        }
                    }
                }

            }

        }

    }

}
