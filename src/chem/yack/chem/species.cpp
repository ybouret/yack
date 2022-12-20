#include "yack/chem/species.hpp"
#include "yack/ios/ostream.hpp"
#include "yack/comparison.hpp"
#include "yack/data/list/sort.hpp"
#include <iomanip>

namespace yack
{
    namespace chemical
    {

        species:: ~species() throw() {}

        species:: species(const species &sp) :
        entity(sp),
        z(sp.z),
        rank(sp.rank)
        {
        }

        std::ostream & species:: frame(std::ostream &os, const readable<double> &C) const
        {
            assert(C.size()>=**this);
            os << std::setw(15) << C[ **this ];
            return os;
        }

        ios::ostream & species:: frame(ios::ostream &os, const readable<double> &C) const
        {
            assert(C.size()>=**this);
            os("%.15g",C[ **this ]);
            return os;
        }

        const char * species:: attr() const
        {
            switch(rank)
            {
                case 0:
                    return ",shape=oval,style=dotted";

                case 1:
                    return ",shape=oval,style=bold";

                default:
                    break;
            }
            return ",shape=oval";
        }




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


    }

}
