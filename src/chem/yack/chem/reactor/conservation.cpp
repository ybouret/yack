#include "yack/chem/reactor.hpp"
#include "yack/chem/eqs/composite.hpp"
#include "yack/math/iota.hpp"

#include "yack/system/imported.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/signs.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        static inline void notConserved(imatrix &NuA, const size_t j) throw()
        {
            for(size_t i=NuA.rows;i>0;--i) NuA[i][j] = 0;
        }

        void reactor:: conservation(const xmlog &xml)
        {
            YACK_XMLSUB(xml, "Conservation");

            NuA.assign(Nu);
            for(const anode *an=working.head;an;an=an->next)
            {
                const species &s = **an;
                const size_t   j = *s;
                const islot   &l = held_by[j];

                if(verbose) corelib.pad(*xml<< s.name,s) << " : " << l << std::endl;
                assert(s.rank==l.size);

                bool    keep = true;
                size_t  n    = 0;
                size_t  p    = 0;

                for(const enode *en=singles.head();en;en=en->next)
                {
                    const equilibrium &eq = ***en;
                    const size_t       i  = *eq;
                    const feature      f  = eq.kind();

                    switch( __sign::of(Nu[i][j]) )
                    {
                        case __zero__:
                            continue;

                        case positive:
                            ++p;
                            break;

                        case negative:
                            ++n;
                            break;
                    }

                    if(p>1||n>1)
                    {
                        // not a conservative equilibrium
                        keep = false;
                        YACK_XMLOG(xml,"|_shared");
                        notConserved(NuA,j);
                        goto DONE;
                    }

                    switch(f)
                    {
                        case undefined:
                            throw exception("undefined equilibrium <%s>", eq.name() );

                        case part_only:
                        case join_only:
                            // not a conservative equilibrium
                            keep = false;
                            YACK_XMLOG(xml,"|_excluded by " << eq.name);
                            notConserved(NuA,j);
                            goto DONE;

                        case both_ways:
                            break;
                    }

                    assert(both_ways==f);
                }

            DONE:
                if(!keep) continue;
                YACK_XMLOG(xml,"|_[" << p << "+|" << n << "-]");




            }

            std::cerr << "Nu  = " << Nu  << std::endl;
            std::cerr << "NuA = " << NuA << std::endl;

            singles.graphviz("eqs.dot",corelib);

            exit(0);
            //"@eq1:-[a]+[b]:1"  "@eq2:-[b]+2[c]:1"  "@eq3:-[c]+[d]+[a]:1"
        }

    }

}

