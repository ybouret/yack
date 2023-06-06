

#include "yack/chem/vat/cluster.hpp"
#include "yack/system/imported.hpp"
#include "yack/data/list/sort.hpp"
#include <iomanip>

namespace yack
{
    namespace Chemical
    {

        static inline
        bool areLinkedByConservedSpecies(const Equilibrium                 &lhs,
                                         const Equilibrium                 &rhs,
                                         const readable<Species::Category> &cat)
        {
            for(const cNode *lnode=lhs->head;lnode;lnode=lnode->next)
            {
                const Species &ls = ****lnode;
                const size_t   lj = ls.indx[SubLevel];
                if(Species::Unbounded==cat[lj]) continue;
                for(const cNode *rnode=rhs->head;rnode;rnode=rnode->next)
                {
                    const Species &rs = ****rnode;
                    const size_t   rj = rs.indx[SubLevel];
                    if(Species::Unbounded==cat[rj]) continue;
                    if( &rs == &ls )
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        void Cluster:: gatherTribes(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"Cluster::gatherTribes");

            const size_t n =  size;
            matrix<bool> indep(n,n);
            {
                indep.ld(false);
                // computing matrix of independents equilibria
                for(const Equilibrium::Node *lhs=head;lhs;lhs=lhs->next)
                {
                    const Equilibrium &L = ***lhs;
                    const size_t       l = L.indx[SubLevel];
                    for(const Equilibrium::Node *rhs=lhs->next;rhs;rhs=rhs->next)
                    {
                        const Equilibrium &R = ***rhs;
                        const size_t       r = R.indx[SubLevel];
                        indep[l][r] = indep[r][l] = !L.linkedTo(R);
                    }
                }

                //if(xml.verbose) for_each_equilibrium( *xml << "indep = ","<",indep,">",SubLevel) << std::endl;

                // build regulating tribes
                YACK_XMLOG(xml, "[[ regulating tribes ]]");
                settleTribes(xml,coerce(regulating),*this,indep);
            }

            {
                // computing matrix of independents standard equilibria
                indep.ld(false);
                for(const Equilibrium::Node *lhs=standard.head;lhs;lhs=lhs->next)
                {
                    const Equilibrium &L = ***lhs;
                    const size_t       l = L.indx[SubLevel];
                    for(const Equilibrium::Node *rhs=lhs->next;rhs;rhs=rhs->next)
                    {
                        const Equilibrium &R = ***rhs;
                        const size_t       r = R.indx[SubLevel];
                        indep[l][r] = indep[r][l] = !areLinkedByConservedSpecies(L,R,category);
                    }
                }
                //if(xml.verbose) for_each_equilibrium( *xml << "indep = ","<",indep,">",SubLevel) << std::endl;

                YACK_XMLOG(xml, "[[ equalizing tribes ]]");
                settleTribes(xml,coerce(equalizing),standard,indep);

            }
            
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
                    for(const Equilibrium::Node *L = lhs->head, *R = rhs->head;
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
                                    const matrix<bool>          &indep)
        {
            YACK_XMLSUB(xml,"settleTribes");
            tribes.release();

            //------------------------------------------------------------------
            // initialize with single equilibria
            //------------------------------------------------------------------
            for(const Equilibrium::Node *node=source.head;node;node=node->next)
            {
                tribes.push_back( new Tribe(***node) );
            }

            //------------------------------------------------------------------
            // try to grow
            //------------------------------------------------------------------
            for(const Equilibrium::Node *node=source.head;node;node=node->next)
            {
                const Equilibrium &curr = ***node;
                for(Tribe *tribe=tribes.head;tribe;tribe=tribe->next)
                {
                    if(tribe->accepts(curr,indep))
                    {
                        tribes.push_front( new Tribe(*tribe,curr) );
                    }
                }
            }

            //------------------------------------------------------------------
            // sort
            //------------------------------------------------------------------
            merge_list_of<Tribe>::sort(tribes,compareTribes);

            //------------------------------------------------------------------
            // finalize
            //------------------------------------------------------------------
            for(Tribe *tribe = tribes.head; tribe; tribe=tribe->next)
            {
                tribe->finalize();
                YACK_XMLOG(xml," (*) #" << std::setw(4) << tribe->size << " @" << *tribe);
            }




        }



    }

}


