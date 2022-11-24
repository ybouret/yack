
#include "yack/chem/reactor.hpp"
#include "yack/chem/eqs/composite.hpp"
#include "yack/system/imported.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/signs.hpp"
#include "yack/associative/addrbook.hpp"
#include "yack/math/iota.hpp"
#include "yack/ptr/auto.hpp"
#include "yack/type/boolean.h"
#include "yack/type/utils.hpp"
#include "yack/apex/kernel.hpp"
#include "yack/math/algebra/crout.hpp"
#include "yack/sequence/list.hpp"
#include "yack/ptr/shared.hpp"
#include "yack/sort/indexing.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        static inline size_t get_coeff_count(const readable<int> &nu) throw()
        {
            size_t     count = 0;
            for(size_t j=nu.size();j>0;--j)
            {
                if(nu[j]) ++count;
            }
            return count;
        }

        void reactor::conservation2(const xmlog &xml)
        {
            static const char fn[] = "conservation";
            YACK_XMLSUB(xml,fn);


            //------------------------------------------------------------------
            //
            //
            // initialize NuA to Nu
            //
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- removing unconserved species" );
            NuA.assign(Nu);
            for(const enode *en=singles.head();en;en=en->next)
            {
                const equilibrium &eq = ***en;

                switch( eq.kind() )
                {
                        // undefined is bad at this point...
                    case undefined: throw exception("%s: undefined <%s>", fn, eq.name() );

                        // remove all involved species
                    case part_only:
                    case join_only:
                        for(const cnode *cn=eq.head();cn;cn=cn->next)
                        {
                            const species &sp = ****cn;
                            const size_t   sj = *sp;
                            for(size_t i=N;i>0;--i) NuA[i][sj] = 0;
                        }
                        break;

                        // ok
                    case both_ways:
                        break;
                }
            }

            if(verbose) std::cerr << "NuA=" << NuA << std::endl;

            //------------------------------------------------------------------
            //
            //
            // counting remaining dimensions
            //
            //
            //------------------------------------------------------------------
            matrix<apq> P;
            {
                // detecting remaining equilibria
                meta_list<const equilibrium> eqdb;
                for(const enode *en=singles.head();en;en=en->next)
                {
                    const equilibrium   &eq = ***en;
                    switch(get_coeff_count(NuA[*eq]))
                    {
                        case 0:
                            break;

                        case 1:
                            throw exception("%s: unexpected remaining 1 species in <%s>",fn,eq.name());

                        default:
                            eqdb << &eq;
                    }
                }

                // check validity
                if(eqdb.size<=0)
                {
                    YACK_XMLOG(xml, "-- no conservation");
                    return;
                }

                // create matrix
                P.make(eqdb.size,M);
                size_t i=1;
                for(const meta_node<const equilibrium> *en=eqdb.head;en;en=en->next,++i)
                {
                    iota::load(P[i],NuA[***en]);
                }
            }
            const size_t Nc = P.rows;
            YACK_XMLOG(xml, "-- look up against Nc=" << Nc);
            if(verbose) std::cerr << "\tP=" << P << std::endl;

            exit(0);
        }


    }

}


