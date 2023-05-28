#include "yack/chem/vat/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace Chemical
    {

        const char Cluster:: CLID[] = "Chemical::Cluster";

        Cluster:: ~Cluster() noexcept {}

        Cluster:: Cluster(Equilibrium &first) :
        EqRepo(),
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
            for(const EqNode *en = cl.head;en;en=en->next)
            {
                const Equilibrium &eq = ***en;
                cl.pad(os << "  <" << eq.name, eq) << ">";
                os << std::endl;
            }
            os << '}';
            return os;
        }


        void Cluster:: finalize(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"Cluster::finalize");
            assert(0==active.size);

            {
                addrbook ab;

                // collect species from renumbered eqs
                {
                    size_t sub = 1;
                    for(const EqNode *en=head;en;en=en->next,++sub)
                    {
                        const Equilibrium &eq = ***en;
                        update(eq);
                        eq.submitTo(ab);
                        coerce(eq.indx[SubLevel]) = sub;
                        if(xml.verbose) *xml << " (+) " << eq.name << std::endl;
                    }
                }


                // collect active from renumber species
                {
                    size_t sub = 1;
                    for(addrbook::const_iterator it=ab.begin();it!=ab.end();++it,++sub)
                    {
                        const Species &sp = *static_cast<const Species *>(*it);
                        active << sp;
                        coerce(sp.indx[SubLevel]) = sub;
                    }
                }
                YACK_XMLOG(xml, " (*) " << active);


            }

            if(size)
            {
                if(active.size <= 0) throw imported::exception(CLID,"invalid equilibria");
                Nu.make(size,active.size);
                for(const EqNode *en=head;en;en=en->next)
                {
                    const Equilibrium &eq = ***en;
                    eq.fill(Nu[ eq.indx[SubLevel] ], SubLevel);
                }
                YACK_XMLOG(xml, " Nu = " << Nu);
            }


        }
    }

}

