#include "yack/chem/sp/list.hpp"
#include "yack/data/list/sort.hpp"

namespace yack {

    namespace chemical
    {

        sp_list::  sp_list() throw() : sp_list_() {}
        sp_list:: ~sp_list() throw() {}

        int sp_list:: node_compare(const sp_node *lhs, const sp_node *rhs) throw()
        {
            assert(lhs);
            assert(rhs);
            return comparison::increasing( ***lhs, ***rhs );
        }


        void sp_list:: sort() throw()
        {
            merge_list_of<sp_node>::sort(*this,node_compare);
        }



        void sp_list:: shed(const sp_list  &other) throw()
        {
            if(this==&other) release();
            else
            {
                sp_list_ kept;
                while(size)
                {
                    sp_node     *node = pop_front();
                    const size_t indx = ***node;
                    bool         flag = false;
                    for(const sp_node *scan=other.head;scan;scan=scan->next)
                    {
                        if(indx==***scan) { flag=true; break; }
                    }
                    if(flag)
                        delete node;
                    else
                        kept.push_back(node);
                }
                swap_with(kept);
            }
        }
    }

}


#include "yack/chem/library.hpp"

namespace yack {
    
    namespace chemical
    {
        void sp_list:: load(const sp_book &tribe, const library &lib)
        {
            sp_list_ &self = *this;
            for(sp_book::const_iterator it=tribe.begin();it!=tribe.end();++it)
            {
                self << &lib[*it];
            }
            sort();
        }
    }
}
