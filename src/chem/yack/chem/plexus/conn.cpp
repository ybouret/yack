
#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/sort/sum.hpp"
#include "yack/arith/gcd.h"
#include "yack/ptr/auto.hpp"

#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        static inline void conn_simp(int &a, int &b) throw()
        {
            assert(a!=0); assert(b!=0);
            const int g = int(yack_gcd64( absolute(a), absolute(b)));
            a/=g;
            b/=g;
        }



        void plexus:: computeConn()
        {

            YACK_CHEM_PRINTLN("// <connectivity>");
            vector<uint64_t> ca(M,as_capacity);
            vector<int>      cf(M,0);
            for(const enode *lhs=eqs.head();lhs;lhs=lhs->next)
            {
                const equilibrium   &el = ***lhs;
                const size_t         il = *el;
                const readable<int> &nl = Nu[il];

                YACK_CHEM_PRINTLN("//   connecting " << el.name);

                for(const enode *rhs=eqs.head();rhs;rhs=rhs->next)
                {
                    if(rhs==lhs) continue;
                    const equilibrium   &er = ***rhs;
                    const size_t         ir = *er;
                    const readable<int> &nr = Nu[ir];
                    interactions        &inter = coerce(conn[il][ir]);

                    for(const anode *node=active.head;node;node=node->next)
                    {
                        const species &s = **node;
                        const size_t   j = *s;

                        const int cl = nl[j]; if(!cl) continue;
                        const int cr = nr[j]; if(!cr) continue;;

                        // get coefficients
                        int wl = cr;
                        int wr = -cl;
                        if(wl<0)
                        {
                            wl = -wl;
                            wr = -wr;
                        }
                        conn_simp(wl,wr);
                        YACK_CHEM_PRINTLN("//     " << wl << " " << el.name << " and " << wr << " " << er.name);

                        // check gcd
                        ca.free();
                        for(size_t k=M;k>0;--k)
                        {
                            const uint64_t u = absolute(cf[k] = wl * nl[k] + wr * nr[k]);
                            if(u) ca.push_back_fast(u);
                        }
                        assert(ca.size()>0);
                        uint64_t g = ca.back(); ca.pop_back();
                        while(ca.size())
                        {
                            g = yack_gcd64(g,ca.back());
                            ca.pop_back();
                        }
                        if(g>1)
                        {
                            for(size_t k=M;k>0;--k)
                            {
                                cf[k] /= g;
                            }
                        }
                        YACK_CHEM_PRINTLN("//     " << cf << " gcd=" << g);

                        // create interaction
                        interaction *I = inter.push_back( new interaction(el,wl,er,wr,int(g)));

                        // fill it with coefficients
                        for(const anode *scan=active.head;scan;scan=scan->next)
                        {
                            const species &S = **scan;
                            const size_t   J = *S;
                            const int      Y = cf[J];
                            if(Y)
                            {
                                I->add(S,Y);
                            }
                        }
                        YACK_CHEM_PRINTLN("//     " << *I);

                     }
                }

                
            }
            YACK_CHEM_PRINTLN("// <connectivity/>");


        }
    }

}

