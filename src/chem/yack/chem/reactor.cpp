#include "yack/chem/reactor.hpp"
#include "yack/chem/eqs/composite.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/algebra/crout.hpp"

#include "yack/apex.hpp"
#include "yack/system/imported.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        const char reactor:: clid[]  = "chemical::reactor";
        bool      &reactor:: verbose = entity::verbose;

        reactor:: ~reactor() throw() {}

        reactor:: reactor(const library    &usrLib,
                          const equilibria &usrEqs,
                          const double      t) :
        corelib(usrLib),
        singles(usrEqs),
        working(corelib.head()),
        M(corelib.size()),
        N(singles.size()),
        L(0),
        couples(),
        lattice(singles),
        related(),
        solving(),
        xmul(),
        xadd(),
        solv(N),
        worklib(corelib),
        mtab(12,M),
        ntab(12,N),
        ltab(12,L),

        Nu(N,N>0?M:0),
        NuA(Nu.rows,Nu.cols),
        Psi(Nu.rows,Nu.cols),
        Omega(N,N),
        iOmega(N,N),
        
        Corg( mtab.next() ),
        Cend( mtab.next() ),
        Ctry( mtab.next() ),
        dC(   mtab.next() ),

        K(     ntab.next()  ),
        Gamma( ntab.next()  ),
        xi(    ntab.next()  ),
        sigma( ntab.next()  ),

        Kl( ltab.next()  ),
        Xl( ltab.next()  ),
        blocked( ltab.next(), transmogrify),
        Ceq(),
        ratio(M,as_capacity),

        lockLib( coerce(usrLib) ),
        lockEqs( coerce(usrEqs) )
        {
            static const char fn[] = "[reactor] ";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            equilibrium::display_time = t;

            YACK_XMLSUB(xml,"BuildingReactor");
            YACK_XMLOG(xml, "@" << t );
            YACK_XMLOG(xml, "corelib = " << corelib);


            if(N>0)
            {
                //--------------------------------------------------------------
                // initialize K and Nu
                //--------------------------------------------------------------
                {
                    YACK_XMLSUB(xml,"Singles");
                    YACK_XMLOG(xml,singles);
                    for(const enode *node=singles.head();node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        const size_t       ei = *eq;
                        K[ei] = eq.K(t);
                        eq.fill( coerce(Nu[ei]) );
                    }

                    if(verbose)
                    {
                        singles(std::cerr, "Nu_", Nu);
                    }

                }

                //--------------------------------------------------------------
                // testing system
                //--------------------------------------------------------------
                {
                    crout<apq>  apLU(N);
                    matrix<apq> Gram(N,N);
                    iota::gram(Gram,Nu);
                    if(!apLU.build(Gram)) throw imported::exception(clid,"singular equilibria");
                }

                //--------------------------------------------------------------
                // compute couples
                //--------------------------------------------------------------
                {
                    YACK_XMLSUB(xml,"Couples");
                    composite::scatter( coerce(couples), worklib, singles, K, xmul);


                    //--------------------------------------------------------------
                    // complete lattice
                    //--------------------------------------------------------------
                    coerce(lattice).add(couples);
                    coerce(L) = lattice.size();

                    if(verbose)
                    {
                        std::cerr << "lattice = " << lattice << std::endl;
                    }
                }
                //--------------------------------------------------------------
                // rebuild ltab
                //--------------------------------------------------------------
                ltab.make(L);
                blocked.relink<bool>();
                Ceq.make(L,M);

                assert(L==Kl.size());
                assert(L==blocked.size());

                //--------------------------------------------------------------
                // initialize Kl
                //--------------------------------------------------------------
                for(size_t i=N;i>0;--i)
                {
                    Kl[i] = K[i];
                }

                //--------------------------------------------------------------
                // complete Kl @t
                //--------------------------------------------------------------
                for(const enode *node=couples.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei =  *eq;
                    Kl[ei] = eq.K(-1);
                }

                //--------------------------------------------------------------
                // build related groups
                //--------------------------------------------------------------
                {
                    YACK_XMLSUB(xml,"Related");
                    buildRelated();
                    if(verbose)
                    {
                        std::cerr << "related = " << related << std::endl;
                    }
                }


                //--------------------------------------------------------------
                // for each related group...
                //--------------------------------------------------------------
                makeManifold(xml);
            }

            
        }

    }

}

