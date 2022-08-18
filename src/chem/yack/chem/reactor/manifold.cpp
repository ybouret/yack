
#include "yack/chem/reactor.hpp"
#include "yack/counting/mloop.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    
    namespace chemical
    {

        void reactor:: makeManifold()
        {
            static const char fn[] = "[reactor.manifold] ";

            //------------------------------------------------------------------
            //
            //
            // initialize local data
            //
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(fn << "[build all detached]");
            const size_t     P = related.size; // number of related
            matrix<bool>     detached(L,L);    // global symetric detached flag
            vector<group>    unlinked(L);      // possible unlinked groups for each equilibrium
            vector<groups>   party(P);         // collect possibilities per related choid
            unsigned         count  = 0;       // count index

            

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
                    if(verbose)
                        lattice.pad(std::cerr << LHS.name,LHS) << " : " << LGP << std::endl;
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
                    (*G.push_back( new group() )) << &LHS;


                    //----------------------------------------------------------
                    // try growing G from all detached equilibrium of LHS
                    //----------------------------------------------------------
                    for(const gnode *rhs=LGP.head;rhs;rhs=rhs->next)
                    {
                        const equilibrium    &RHS = **rhs;
                        const size_t          RID = *RHS;
                        const readable<bool> &TST = detached[RID];

                        for(const group *existing=G.head;existing;existing=existing->next)
                        {
                            // check detached from all members of existing group
                            bool ok = true;
                            for(const gnode *member=existing->head;member;member=member->next)
                            {
                                if(!TST[***member])
                                {
                                    ok = false;
                                    break;
                                }
                            }
                            if(ok)
                            {
                                // duplicate and grow current
                                (*G.push_front( new group(*existing) ))<< &RHS;
                            }
                        }
                    }

                    target.merge_back(G);
                }

                YACK_CHEM_PRINTLN(target);
                YACK_CHEM_PRINTLN(fn << "-------- quit related #" << count << " --------");
            }

            //------------------------------------------------------------------
            //
            //
            // combination of parties
            //
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(fn << " --> combining parts <--");
            {
                const vector<size_t>    ini(P,1);
                vector<size_t>          end(P,1);
                for(size_t i=P;i>0;--i) end[i] = party[i].size;
                mloop<size_t> loop(ini(),end(),P);
                do
                {
                    group *g = coerce(solving).push_back( new group() );
                    for(size_t i=P;i>0;--i)
                    {
                        const size_t k = loop[i];
                        g->merge_back_copy( *party[i].get(k) );
                    }

                } while(loop.next());
            }
            coerce(solving).sort();

            YACK_CHEM_PRINTLN(solving);
            YACK_CHEM_PRINTLN(fn << "[built all detached]=" << solving.size);
            
#ifndef NDEBUG
            YACK_CHEM_PRINTLN(fn << "[*** testing ***]");
            lattice(std::cerr,"",detached);
            for(const enode *I=lattice.head();I;I=I->next)
            {
                const equilibrium &i = ***I;
                //YACK_CHEM_PRINTLN('\t' << i.name);
                if(!solving.includes(i)) throw imported::exception(clid,"missing %s",i.name());
                for(const enode *J=I->next;J;J=J->next)
                {
                    const equilibrium &j = ***J;
                    if(i.detached_of(j))
                    {
                        group g; g << &i << &j;
                        g.sort();
                        //YACK_CHEM_PRINTLN("\t\t" << g);
                        if(!solving.contains(g)) throw imported::exception(clid,"missing (%s,%s)",i.name(),j.name());
                        for(const enode *K=J->next;K;K=K->next)
                        {
                            const equilibrium &k = ***K;
                            if(k.detached_of(i) && k.detached_of(j))
                            {
                                g << &k;
                                g.sort();
                                //YACK_CHEM_PRINTLN("\t\t\t" << g);
                                if(!solving.contains(g)) throw imported::exception(clid,"missing (%s,%s,%s)",i.name(),j.name(),k.name());
                                delete g.pop_back();
                            }
                        }
                    }
                }
            }
            YACK_CHEM_PRINTLN(fn << "[*** tested ***]");
#endif
            
        }

    }

}


