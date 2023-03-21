
#include "yack/aqueous/realm/domain.hpp"

namespace yack
{
    namespace aqueous
    {
        eq_slots:: ~eq_slots() noexcept {}

        eq_slots:: eq_slots(const size_t n) : object(), eq_slots_(n) {}
    }

}

namespace yack
{
    namespace aqueous
    {

        domain:: ~domain() noexcept
        {
        }

        domain:: domain(const equilibrium      &first,
                        const readable<bool>   &flags,
                        const readable<size_t> &grpid)   :
        spot_object(),
        eq_list(),
        reg(flags),
        grp(grpid),
        N(0),
        M(0),
        L(0),
        last(NULL),
        live(),
        laws(),
        pack(),
        reacting(),
        retaking(),
        combining(),
        splitting(),
        roaming(),
        defined(),
        endless(),
        bounded(),
        Nu(),
        NuT(),
        next(0),
        prev(0),
        iviz(0),
        slots(NULL)
        {
            (*this) << first;
        }

        int domain:: compare(const eq_node *lhs, const eq_node *rhs) noexcept
        {
            const equilibrium &L = ***lhs;
            const equilibrium &R = ***rhs;
            return comparison::increasing(L.indx[0],R.indx[0]);
        }


        bool domain:: accepts(const equilibrium &rhs) const noexcept
        {
            for(const eq_node *en=head;en;en=en->next)
            {
                const equilibrium &lhs = ***en; assert(&lhs!=&rhs);
                if(lhs.is_connected_to(rhs)) return true;
            }
            return false;
        }

        bool domain:: accepts(const domain &rhs) const noexcept
        {
            // loop over rhs eqs
            for(const eq_node *en=rhs.head;en;en=en->next)
            {
                if(accepts(***en)) return true;
            }
            return false;
        }

        void domain:: reg_db(addrbook          &db,
                             const equilibrium &eq) const
        {
            db.free();
            for(const cnode *cn=eq->head;cn;cn=cn->next)
            {
                const species &sp = ****cn;
                if( reg[ sp.indx[top_level]] ) db.ensure(&sp);
            }
        }

        bool domain:: reg_in(const addrbook    &db,
                             const equilibrium &eq) const noexcept
        {
            for(const cnode *cn=eq->head;cn;cn=cn->next)
            {
                const species &sp = ****cn;
                if( !reg[ sp.indx[top_level]] ) continue;
                if(db.search(&sp)) return true;
            }
            return false;
        }

        static inline
        void eq_viz(ios::ostream      &fp,
                    const eq_node     *node,
                    const size_t       rank,
                    const char        *ppty)
        {
            const string _(ppty);
            for(;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                if(rank != eq.rank) continue;
                eq.viz(fp,_);
            }
        }

        static inline
        void collect_species(addrbook          &spdb,
                             const eq_node     *node,
                             const size_t       rank)
        {
            for(;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                if(rank != eq.rank) continue;
                eq.report_to(spdb);
            }
        }



        void domain:: viz(ios::ostream &fp, const size_t rank) const
        {
            assert(slots.is_valid());
            if(slots->size()<rank)        return;
            if( (*slots)[rank].size <= 0) return;



            fp("subgraph cluster_%d{\n",iviz);

            {
                addrbook db;
                collect_species(db, combining.head, rank);
                collect_species(db, splitting.head, rank);
                collect_species(db, roaming.head,   rank);
                collect_species(db, defined.head,   rank);
                for(addrbook::const_iterator it=db.begin();it!=db.end();++it)
                {
                    const species &s = *static_cast<const species *>(*it);
                    const size_t   j = s.indx[top_level];
                    s.viz(fp, reg[j], grp[j]);
                }
            }

            eq_viz(fp,combining.head, rank, ",style=\"bold,dashed\",shape=trapezium");
            eq_viz(fp,splitting.head, rank, ",style=\"bold,dashed\",shape=invtrapezium");
            eq_viz(fp,roaming.head,   rank, ",style=\"bold,dashed\",shape=box");
            eq_viz(fp,defined.head,   rank, ",style=bold,shape=box");


            fp << "}\n";
        }
    }

}
