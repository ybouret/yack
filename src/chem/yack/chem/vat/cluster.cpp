#include "yack/chem/vat/cluster.hpp"
#include "yack/system/imported.hpp"
#include "yack/apex/flak.hpp"

namespace yack
{
    namespace Chemical
    {

        const char Cluster:: CLID[] = "Chemical::Cluster";

        Cluster:: ~Cluster() noexcept {}

        Cluster:: Cluster(Equilibrium &first) :
        EqList(),
        Qrnk(0),
        next(0),
        prev(0)
        {
            (*this) << first;
        }


        bool Cluster:: recruits(const Equilibrium &other) const noexcept
        {
            for(const EqNode *en=head;en;en=en->next)
            {
                const Equilibrium &eq = ***en;
                if(eq.linkedTo(other)) return true;
            }
            return false;
        }

        bool Cluster:: linkedTo(const Cluster &other) const noexcept
        {
            for(const EqNode *en=head;en;en=en->next)
            {
                const Equilibrium &lhs = ***en;
                for(const EqNode *on=other.head;on;on=on->next)
                {
                    const Equilibrium &rhs = ***on;
                    assert(&rhs != &lhs);
                    if(lhs.linkedTo(rhs)) return true;
                }
            }
            return false;
        }

        std::ostream & operator<<( std::ostream &os, const Cluster &cl)
        {
            os << '{' << std::endl;
            for(const EqNode *node=cl.head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                cl.pad(os << " <" << eq.name,eq) << "> : ";
                cl.reac.pad(os << eq.reac.name,eq.reac);
                os << " <=> ";
                cl.prod.pad(os << eq.prod.name,eq.prod);
                os << " : ";
                os << std::endl;
            }
            os << '}';
            return os;
        }



        void Cluster:: finalize(const xmlog &xml,
                                Equilibria  &eqs)
        {
            YACK_XMLSUB(xml,"Cluster::finalize");
            assert(0==lib.size);
            
            {
                addrbook ab;

                //--------------------------------------------------------------
                //
                // collect species and renumber eqs
                //
                //--------------------------------------------------------------
                {
                    size_t sub = 1;
                    for(const EqNode *en=head;en;en=en->next,++sub)
                    {
                        const Equilibrium &eq = ***en;
                        update(eq);
                        if(eq->size<=0)   throw imported::exception(CLID,"empty <%s>",       eq.name());
                        if(!eq.neutral()) throw imported::exception(CLID,"not neutral <%s>", eq.name());
                        if(!eq.minimal()) throw imported::exception(CLID,"not minimal <%s>", eq.name());
                        eq.submitTo(ab);
                        coerce(eq.indx[SubLevel]) = sub;
                        if(xml.verbose) *xml << " (+) " << eq.name << std::endl;
                    }
                }


                //--------------------------------------------------------------
                //
                // collect lib from renumber species
                //
                //--------------------------------------------------------------
                {
                    size_t sub = 1;
                    for(addrbook::const_iterator it=ab.begin();it!=ab.end();++it,++sub)
                    {
                        const Species &sp = *static_cast<const Species *>(*it);
                        coerce(lib) << sp;
                        coerce(sp.indx[SubLevel]) = sub;
                    }
                }
                YACK_XMLOG(xml, " (*) " << lib);



            }

            if(size)
            {
                //--------------------------------------------------------------
                //
                // create principal topology
                //
                //--------------------------------------------------------------
                assert(lib.size>0);
                coerce(Nu).make(size,lib.size);
                for(const EqNode *en=head;en;en=en->next)
                {
                    const Equilibrium &eq = ***en;
                    eq.fill( coerce(Nu)[ eq.indx[SubLevel] ], SubLevel);
                }
                YACK_XMLOG(xml, " Nu = " << Nu);

                const size_t rank = apex::flak::rank(Nu);
                if(size!=rank)
                    throw imported::exception(CLID,"system rank=%u < size=%u", unsigned(rank), unsigned(size));

                //--------------------------------------------------------------
                //
                // finding out conservation laws
                //
                //--------------------------------------------------------------
                discoverLaws(xml);

                //--------------------------------------------------------------
                //
                // build manifold
                //
                //--------------------------------------------------------------
                makeManifold(xml,eqs);
            }




        }
    }

}

#include "yack/chem/library.hpp"

namespace yack
{
    namespace Chemical
    {
        void Cluster:: fill(writable<double> &C,
                            randomized::bits &ran,
                            const IndexLevel  level) const noexcept
        {
            for(const SpNode *node=lib.head;node;node=node->next)
            {
                const Species &sp = ***node;
                C[ sp.indx[level] ] = Library::Conc(ran,0);
            }
        }
    }
}

