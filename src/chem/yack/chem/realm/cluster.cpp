#include "yack/chem/realm/cluster.hpp"


namespace yack
{
    namespace chemical
    {

        cluster:: ~cluster() noexcept {}

        cluster:: cluster(const equilibrium &first) :
        eq_list(),
        spec(),
        next(0),
        prev(0)
        {
            (*this) << first;
        }

        cluster:: cluster(const cluster &other, const equilibrium &extra) :
        eq_list(other),
        spec(other.spec),
        next(0),
        prev(0)
        {
            (*this) << extra;
            assert(check());
        }

        bool cluster:: accepts(const matrix<bool> &detached, const equilibrium &extra) const noexcept
        {
            assert(NULL!=tail);
            const size_t          t  = (***tail).indx[sub_level];
            const size_t          i  = extra.indx[sub_level]; if(i<=t) return false;
            const readable<bool> &d  = detached[i];
            for(const eq_node *en=head;en;en=en->next)
            {
                if( !d[ (***en).indx[sub_level] ] ) return false;
            }
            return true; // detached from all
        }

        bool cluster:: check() const noexcept
        {
            if(size<=0) { std::cerr << "empty cluster" << std::endl; return false; }

            for(const eq_node *node=head;node->next;node=node->next)
            {
                const equilibrium &lhs = ***node;
                const equilibrium &rhs = ***(node->next);
                if(rhs.indx[sub_level]<=lhs.indx[sub_level])
                {
                    std::cerr << "wrong cluster ordering" << std::endl;
                    return false;
                }
            }

            return true;
        }

        int cluster:: compare(const cluster *lhs, const cluster *rhs) noexcept
        {
            assert(lhs);
            assert(rhs);

            switch( __sign::of(lhs->size,rhs->size) )
            {
                case __zero__: break;
                case negative: return -1;
                case positive: return 1;
            }

            for(const eq_node *L=lhs->head, *R=rhs->head; L; L=L->next,R=R->next)
            {
                switch( __sign::of( (***L).indx[sub_level], (***R).indx[sub_level] ) )
                {
                    case __zero__: continue;
                    case negative: return -1;
                    case positive: return  1;
                }
            }
            return 0;
        }

#if 0
        bool cluster:: is_subset_of(const eq_repo &eqs) const noexcept
        {
            for(const eq_node *node=head;node;node=node->next)
            {
                if( !eqs.contains( ***node) ) return false;
            }
            return true;
        }
#endif
        
        void cluster:: compile()
        {
            // gather species
            addrbook db;
            for(const eq_node *node=head;node;node=node->next)
                (***node).report_to(db);

            // record them
            sp_list &mine = coerce(spec);
            mine.clear();
            for(addrbook::const_iterator it=db.begin();it!=db.end();++it)
                mine << *static_cast<const species *>(*it);

            // order them
            sort_top_level(mine);
            
        }


    }
}