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
            // create reactive partition
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


        }
    }

}

