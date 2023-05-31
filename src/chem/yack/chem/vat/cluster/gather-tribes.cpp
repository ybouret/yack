

#include "yack/chem/vat/cluster.hpp"
#include "yack/system/imported.hpp"
#include "yack/data/list/sort.hpp"

namespace yack
{
    namespace Chemical
    {
        void Cluster:: gatherTribes(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"Cluster::gatherTribes");

            const size_t n =  size;
            matrix<bool> separated(n,n);
            for(const Equilibrium::Node *lhs=head;lhs;lhs=lhs->next)
            {
                const Equilibrium &L = ***lhs;
                const size_t       l = L.indx[SubLevel];
                separated[l][l] = false;
                for(const Equilibrium::Node *rhs=lhs->next;rhs;rhs=rhs->next)
                {
                    const Equilibrium &R = ***rhs;
                    const size_t       r = R.indx[SubLevel];
                    separated[l][r] = separated[r][l] = !L.linkedTo(R);
                }
            }

            if(xml.verbose)
                for_each_equilibrium( *xml << "separated = ","<",separated,">",SubLevel) << std::endl;

            settleTribes(xml,coerce(regulating),*this,separated);
            
        }


        static inline int compareTribes(const Tribe *lhs, const Tribe *rhs)
        {
            const size_t l = lhs->size;
            const size_t r = rhs->size;
            if(l<r)
            {
                return -1;
            }
            else
            {
                if(r<l)
                {
                    return 1;
                }
                else
                {
                    for(const TribeNode *L = lhs->head, *R = rhs->head;
                        L;
                        L=L->next,R=R->next)
                    {
                        assert(R);
                        switch(__sign::of( (***L).indx[SubLevel], (***R).indx[SubLevel]) )
                        {
                            case negative: return -1;
                            case __zero__: continue;
                            case positive: return 1;
                        }
                    }

                    return 0;
                }
            }
        }

        void Cluster:: settleTribes(const xmlog                 &xml,
                                    Tribes                      &tribes,
                                    const Equilibrium::CoreRepo &source,
                                    const matrix<bool>          &separated)
        {
            YACK_XMLSUB(xml,"settleTribes");
            tribes.release();

            // initialize
            for(const Equilibrium::Node *node=source.head;node;node=node->next)
            {
                tribes.push_back( new Tribe(***node) );
            }


            // try to grow
            for(const Equilibrium::Node *node=source.head;node;node=node->next)
            {
                const Equilibrium &curr = ***node;
                for(Tribe *tribe=tribes.head;tribe;tribe=tribe->next)
                {
                    if(tribe->accepts(curr,separated))
                    {
                        tribes.push_front( new Tribe(*tribe,curr) );
                    }
                }
            }

            // sort
            merge_list_of<Tribe>::sort(tribes,compareTribes);

            if(xml.verbose)
            {
                for(const Tribe *tribe = tribes.head; tribe; tribe=tribe->next)
                {
                    *xml << " (*) " << *tribe << std::endl;
                }
            }


        }



    }

}


