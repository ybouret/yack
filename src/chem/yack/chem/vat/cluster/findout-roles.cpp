
#include "yack/chem/vat/cluster.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace Chemical
    {
        namespace
        {
            struct ActorCount
            {
                size_t conserved;
                size_t unbounded;
                size_t extensive;

#if 0
                inline friend std::ostream & operator<<(std::ostream &os, const ActorCount &self)
                {
                    os << "(extensive=" << self.extensive << ", conserved=" << self.conserved << ", unbounded=" << self.unbounded << ")";
                    return os;
                }
#endif

                static inline void Count(ActorCount                        &count,
                                         const Actors                      &actors,
                                         const readable<Species::Category> &category)
                {
                    assert( out_of_reach::is0(&count,sizeof(ActorCount)) );
                    count.extensive = actors.size;
                    for(const Actor *ac=actors.head;ac;ac=ac->next)
                    {
                        switch(category[(**ac).indx[SubLevel]])
                        {
                            case Species::Conserved: ++count.conserved; break;
                            case Species::Unbounded: ++count.unbounded; break;
                        }
                    }
                    assert(count.conserved+count.unbounded==count.extensive);
                }

                static inline
                void Get(ActorCount                        &reac,
                         ActorCount                        &prod,
                         const Equilibrium                 &eq,
                         const readable<Species::Category> &category)
                {
                    assert( out_of_reach::is0(&reac,sizeof(ActorCount)) );
                    assert( out_of_reach::is0(&prod,sizeof(ActorCount)) );
                    Count(reac,eq.reac,category);
                    Count(prod,eq.prod,category);
                }
            };



        }


        void Cluster:: findOutRoles(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"Cluster::findOutRoles");
            coerce(category).make(lib.size,Species::Unbounded);



            //------------------------------------------------------------------
            //
            // check from conservation laws
            //
            //------------------------------------------------------------------
            for(const ConservationLaw *law=laws.head;law;law=law->next)
            {
                for(const Actor *ac=law->head;ac;ac=ac->next)
                {
                    const Species &sp = **ac;
                    coerce(category)[ sp.indx[SubLevel] ] = Species::Conserved;
                }
            }

            //------------------------------------------------------------------
            //
            // build lists from databases
            //
            //------------------------------------------------------------------
            for(const Species::Node *node=lib.head;node;node=node->next)
            {
                const Species &sp = ***node;
                switch(category[sp.indx[SubLevel]])
                {
                    case Species::Unbounded: coerce(unbounded) << sp; break;
                    case Species::Conserved: coerce(conserved) << sp; break;
                }
            }


            YACK_XMLOG(xml,"(*) Species");
            YACK_XMLOG(xml," |_conserved = " << conserved);
            YACK_XMLOG(xml," |_unbounded = " << unbounded);

            assert(lib.size==conserved.size+unbounded.size);

            //------------------------------------------------------------------
            //
            // sorting out eqs
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml,"(*) Equilibria");
            for(const Equilibrium::Node *node=head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                ActorCount  nr = { 0,0,0 };
                ActorCount  np = { 0,0,0 };
                ActorCount::Get(nr,np, eq, category);
                const char *id = NULL;


                if(nr.extensive<=0)
                {
                    assert(nr.conserved<=0); assert(nr.unbounded<=0);
                    assert(np.extensive>0);  assert(np.unbounded==np.extensive); assert(np.conserved==0);
                    coerce(prodOnly) << eq;
                    id = "prodOnly";
                    goto END;
                }

                if(np.extensive<=0)
                {
                    assert(np.conserved<=0); assert(np.unbounded<=0);
                    assert(nr.extensive>0);  assert(nr.unbounded==nr.extensive); assert(nr.conserved==0);
                    coerce(prodOnly) << eq;
                    id = "reacOnly";
                    goto END;
                }

                assert(nr.extensive>0);
                assert(np.extensive>0);

                static const unsigned ConservedNone = 0x00;
                static const unsigned ConservedReac = 0x01;
                static const unsigned ConservedProd = 0x02;
                static const unsigned ConservedBoth = ConservedReac | ConservedProd;
                {
                    unsigned flag = ConservedNone;
                    if(nr.conserved>0) flag |= ConservedReac;
                    if(np.conserved>0) flag |= ConservedProd;

                    switch(flag)
                    {
                        case ConservedNone: coerce(nebulous) << eq; id="nebulous"; goto END;
                        case ConservedBoth: coerce(standard) << eq; id="standard"; goto END;
                        default:
                            break;
                    }
                }
                throw imported::exception(CLID,"case shouldn't exist!!");


            END:
                assert(NULL!=id);
                YACK_XMLOG(xml, " |_[" << id << "] " << eq.name);

            }


        }

    }

}


