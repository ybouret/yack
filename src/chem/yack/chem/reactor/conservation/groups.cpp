
#include "yack/chem/reactor/conservation/groups.hpp"

namespace yack
{
    namespace chemical
    {
        cl_groups:: cl_groups() throw() :
        object(),
        cl_groups_()
        {

        }

        cl_groups:: ~cl_groups() throw()
        {
        }

        void cl_groups:: recruit(const claw &cl)
        {
            //------------------------------------------------------------------
            //
            // find direct fusion
            //
            //------------------------------------------------------------------
            bool direct = false;
            for(cl_group *g=head;g;g=g->next)
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
                push_back( new cl_group(cl) );
            }
            else
            {
                //--------------------------------------------------------------
                // recompile
                //--------------------------------------------------------------
                cl_groups_ temp;
                while(size)
                {
                    auto_ptr<cl_group> g = pop_front();
                    for(cl_group *h=temp.head;h;h=h->next)
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


