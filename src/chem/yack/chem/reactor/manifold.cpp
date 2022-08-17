
#include "yack/chem/reactor.hpp"

namespace yack
{
    
    namespace chemical
    {

        template <typename SEQUENCE, typename ARGS> static inline
        void fill_seq(SEQUENCE &seq, const ARGS &args, size_t n)
        {
            while(n-- >0 ) seq.push_back(args);
        }

        void reactor:: make_manifold()
        {
            static const char fn[] = "[reactor.manifold] ";

            //------------------------------------------------------------------
            //
            //
            // initialize local data
            //
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(fn << "building all detached...");
            const size_t     P = related.size;           // number of related
            matrix<bool>     detached(L,L);              // global symetric detached flag
            vector<group>    unlinked(L,as_capacity);    // possible unlinked groups for each equilibrium
            vector<groups>   party(P,as_capacity);       // collect possibilities per related choid
            unsigned         count  = 0;                 // count index

            { const group  zgrp;  fill_seq(unlinked,zgrp,L); }
            { const groups zgrps; fill_seq(party,zgrps,P);   }

            //------------------------------------------------------------------
            //
            //
            // outer loop over related parts
            //
            //
            //------------------------------------------------------------------
            for(const group *source = related.head; source; source=source->next)
            {

                //--------------------------------------------------------------
                //
                // intialize all possible combinations within the source
                //
                //--------------------------------------------------------------
                ++count;
                YACK_CHEM_PRINTLN(fn << "-------- init related #" << count << " --------");
                groups &target = party[count];
                for(size_t i=L;i>0;--i) unlinked[i].release();


                //--------------------------------------------------------------
                //
                // create unlinked list and fill detached matrix
                //
                //--------------------------------------------------------------
                for(const gnode *lhs=source->head;lhs;lhs=lhs->next)
                {
                    const equilibrium &LHS = **lhs;         // current equilibrium
                    const size_t       LID = *LHS;          // its index
                    group             &LGP = unlinked[LID]; // temporary group
                    writable<bool>    &ROW = detached[LID]; // detached row

                    for(const gnode *rhs = lhs->next;rhs;rhs=rhs->next)
                    {
                        const equilibrium &RHS = **rhs;
                        const size_t       RID = *RHS;

                        if( LHS.detached_of(RHS) )
                        {
                            LGP           << &RHS;                // store into temporary group
                            ROW[RID] = detached[RID][LID] = true; // update detached state
                        }
                    }
                    lattice.pad(std::cerr << LHS.name,LHS) << " : +" << LGP.size << " :" << LGP << std::endl;
                }

                //--------------------------------------------------------------
                //
                // create all possibilities
                //
                //--------------------------------------------------------------
                for(const gnode *lhs=source->head;lhs;lhs=lhs->next)
                {
                    //----------------------------------------------------------
                    // initialize
                    //----------------------------------------------------------
                    groups             G;             // local list of possible groups
                    const equilibrium &LHS = **lhs;
                    const size_t       LID = *LHS;
                    const group       &LGP = unlinked[LID];

                    //----------------------------------------------------------
                    // create singleton
                    //----------------------------------------------------------
                    std::cerr << "\tstarting from " << LHS.name << std::endl;
                    (*G.push_back( new group() )) << &LHS;


                    //----------------------------------------------------------
                    // try growing G from all detached equilibrium of LHS
                    //----------------------------------------------------------
                    for(const gnode *rhs=LGP.head;rhs;rhs=rhs->next)
                    {
                        const equilibrium   &RHS  = **rhs;
                        const size_t         RID  = *RHS;
                        const readable<bool> &TST = detached[RID];
                        //std::cerr << "\t\tusing " << RHS.name << std::endl;

                        for(const group *existing=G.head;existing;existing=existing->next)
                        {
                            // check detached from all current group
                            //std::cerr << "\t\tagainst " << *existing;
                            bool ok = true;
                            for(const gnode *member=existing->head;member;member=member->next)
                            {
                                if(!TST[***member])
                                {
                                    ok = false;
                                    break;
                                }
                            }
                            //std::cerr << " ok=" << ok << std::endl;
                            if(ok)
                            {
                                // duplicate and grow current
                                (*G.push_front( new group(*existing) ))<< &RHS;
                            }
                        }
                    }


                    target.merge_back(G);
                }
                target.sort();
                std::cerr << target << std::endl;
                YACK_CHEM_PRINTLN(fn << "-------- quit related #" << count << " --------");

            }

            std::cerr << party << std::endl;

            exit(1);



            

        }

    }

}


