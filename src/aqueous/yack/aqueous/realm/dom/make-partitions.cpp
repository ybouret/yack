#include "yack/aqueous/realm/domain.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace aqueous
    {
        static inline
        void create_partition(partition          &part,
                              const eq_list      &self,
                              const matrix<bool> &detached)
        {

            //------------------------------------------------------------------
            // initialize list
            //------------------------------------------------------------------
            for(const eq_node *en=self.head;en;en=en->next)
            {
                const equilibrium &first = ***en;
                part.push_back( new cluster(first) );
            }

            //------------------------------------------------------------------
            // try to duplicate/add possibilities
            //------------------------------------------------------------------
            for(const eq_node *en=self.head;en;en=en->next)
            {
                const equilibrium    &extra = ***en;
                for(cluster *cls=part.head;cls;cls=cls->next)
                {
                    if(cls->accepts(detached,extra))
                    {
                        part.push_front( new cluster(*cls,extra) );
                    }
                }
            }

            //------------------------------------------------------------------
            // orderiong
            //------------------------------------------------------------------
            merge_list_of<cluster>::sort(part,cluster::compare);

        }

        static inline
        bool found_common_regular_in(const addrbook       &db,
                                     const equilibrium    &eq,
                                     const readable<bool> &reg) noexcept
        {
            for(const cnode *cn=eq->head;cn;cn=cn->next)
            {
                const species &sp = ****cn;
                if( !reg[ sp.indx[top_level]] ) continue;
                if(db.search(&sp)) return true;
            }
            return false;
        }

        void domain::make_partitions(const xmlog &xml)
        {
            YACK_XMLSUB(xml, "make_partitions");
            assert(size     ==L);
            matrix<bool> detached(L,L);

            //------------------------------------------------------------------
            // create full detached matrix
            //------------------------------------------------------------------
            for(const eq_node *lhs=head;lhs;lhs=lhs->next)
            {
                const equilibrium &L = ***lhs;
                const size_t       l = L.indx[sub_level];
                detached[l][l] = false;
                for(const eq_node *rhs=lhs->next;rhs;rhs=rhs->next)
                {
                    const equilibrium &R = ***rhs;
                    const size_t       r = R.indx[sub_level];
                    detached[r][l] = detached[l][r] = !L.is_connected_to(R);
                }
                //YACK_XMLOG(xml, detached[l] << " @" << L);
            }

            //------------------------------------------------------------------
            // create reacting partition
            //------------------------------------------------------------------
            create_partition( coerce(reacting), *this, detached);

            if(xml.verbose)
            {
                *xml << "-------- reacting --------" << std::endl;
                for(const cluster *cls=reacting.head;cls;cls=cls->next)
                {
                    *xml << *cls << std::endl;
                }
                *xml << "|reacting|=" << reacting.size << std::endl;
            }

            //------------------------------------------------------------------
            // create retaking partition
            //------------------------------------------------------------------
            detached.ld(false);
            addrbook db;
            for(const eq_node *lhs=defined.head;lhs;lhs=lhs->next)
            {
                const equilibrium &L = ***lhs;
                const size_t       l = L.indx[sub_level];
                db.free();
                for(const cnode *cn=L->head;cn;cn=cn->next)
                {
                    const species &sp = ****cn;
                    if( reg[ sp.indx[top_level]] ) db.ensure(&sp);
                }

                for(const eq_node *rhs=lhs->next;rhs;rhs=rhs->next)
                {
                    const equilibrium &R = ***rhs;
                    const size_t       r = R.indx[sub_level];
                    if(found_common_regular_in(db,R,reg))
                        continue;
                    detached[l][r] = detached[r][l] = true;
                }
                YACK_XMLOG(xml, detached[l] << " @" << L);
            }
            create_partition( coerce(retaking), defined, detached);
            
            if(xml.verbose)
            {
                *xml << "-------- retaking --------" << std::endl;
                for(const cluster *cls=retaking.head;cls;cls=cls->next)
                {
                    *xml << *cls << std::endl;
                }
                *xml << "|retaking|=" << retaking.size << std::endl;
            }
        }
    }

}

