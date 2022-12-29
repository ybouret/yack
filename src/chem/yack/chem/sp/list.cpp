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

        void sp_list:: load(const addrbook &tribe)
        {
            sp_list_ &self = *this;
            for(addrbook::const_iterator it=tribe.begin();it!=tribe.end();++it)
            {
                self << static_cast<const species*>(*it);
            }
            sort();
        }
    }

}
