#include "yack/chem/realm/domain.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace chemical
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
            // ordering
            //------------------------------------------------------------------
            merge_list_of<cluster>::sort(part,cluster::compare);

            //------------------------------------------------------------------
            // compiling
            //------------------------------------------------------------------
            for(cluster *cls=part.head;cls;cls=cls->next)
            {
                cls->compile();
            }

        }


        void domain::make_partitions(const xmlog &xml)
        {
            YACK_XMLSUB(xml, "make_partitions");
            assert(size     ==L);
            matrix<bool> detached(L,L);

            //------------------------------------------------------------------
            // create full detached matrix
            //------------------------------------------------------------------
            {
                YACK_XMLSUB(xml, "reactive_clusters");
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
                    for(const cluster *cls=reacting.head;cls;cls=cls->next)
                    {
                        *xml << *cls << std::endl;
                    }
                    *xml << "|reacting|=" << reacting.size << std::endl;
                }
            }

            //------------------------------------------------------------------
            // create retaking partition
            //------------------------------------------------------------------
            {
                YACK_XMLSUB(xml, "retaking_clusters");
                detached.ld(false);
                addrbook db;
                for(const eq_node *lhs=defined.head;lhs;lhs=lhs->next)
                {
                    const equilibrium &L = ***lhs;
                    const size_t       l = L.indx[sub_level];
                    reg_db(db,L);
                    for(const eq_node *rhs=lhs->next;rhs;rhs=rhs->next)
                    {
                        const equilibrium &R = ***rhs; if(reg_in(db,R)) continue;
                        const size_t       r = R.indx[sub_level];
                        detached[l][r] = detached[r][l] = true;
                    }

                    //YACK_XMLOG(xml, detached[l] << " @" << L);
                }

                create_partition( coerce(retaking), defined, detached);

                if(xml.verbose)
                {
                    for(const cluster *cls=retaking.head;cls;cls=cls->next)
                    {
                        *xml << *cls << std::endl;
                    }
                    *xml << "|retaking|=" << retaking.size << std::endl;
                }
            }

        }
    }

}

