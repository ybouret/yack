
#include "yack/chem/reactor/conservation/groups.hpp"

namespace yack
{
    namespace chemical
    {
        clots:: clots() noexcept :
        object(),
        cxx_list_of<clot>()
        {

        }

        clots:: ~clots() noexcept
        {
        }

        void clots:: recruit(const claw &cl)
        {
            //------------------------------------------------------------------
            //
            // find direct fusion
            //
            //------------------------------------------------------------------
            bool direct = false;
            for(clot *g=head;g;g=g->next)
            {
                if(g->attached_to(cl)) {
                    direct = true;
                    (*g) << cl;
                    break;
                }
            }

            if(!direct)
            {
                //--------------------------------------------------------------
                // create new group
                //--------------------------------------------------------------
                push_back( new clot(cl) );
            }
            else
            {
                //--------------------------------------------------------------
                // recompile
                //--------------------------------------------------------------
                cxx_list_of<clot> temp;
                while(size)
                {
                    auto_ptr<clot> g = pop_front();
                    for(clot *h=temp.head;h;h=h->next)
                    {
                        if(h->attached_to(*g))
                        {
                            h->merge_back(*g); assert(0==g->size);
                            g = NULL;          assert(g.is_empty());
                            break;
                        }
                    }
                    if(g.is_valid()) temp.push_back( g.yield() );
                }
                swap_with(temp);
            }


        }


    }

}


