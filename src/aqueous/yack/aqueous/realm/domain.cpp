
#include "yack/aqueous/realm/domain.hpp"

namespace yack
{
    namespace aqueous
    {
        domain:: ~domain() noexcept
        {
        }

        domain:: domain(const equilibrium    &first,
                        const readable<bool> &flags)   :
        large_object(),
        eq_list(),
        reg(flags),
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
        iviz(0)
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
                    const size_t       imax,
                    const char        *ppty)
        {
            const string _(ppty);
            for(;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                if(eq.indx[sub_level]>imax) continue;
                eq.viz(fp,_);
            }
        }


        void domain:: viz(ios::ostream &fp) const
        {
            fp("subgraph cluster_%d{\n",iviz);
            
            eq_viz(fp,combining.head,N,",style=\"bold,dashed\",shape=trapezium");
            eq_viz(fp,splitting.head,N,",style=\"bold,dashed\",shape=invtrapezium");
            eq_viz(fp,roaming.head,N,",style=\"bold,dashed\",shape=box");
            eq_viz(fp,defined.head,N,",style=bold,shape=box");


            fp << "}\n";
        }
    }

}
