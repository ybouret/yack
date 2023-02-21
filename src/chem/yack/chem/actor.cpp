
#include "yack/chem/actor.hpp"
#include "yack/type/utils.hpp"
#include "yack/arith/ipower.hpp"
#include <iomanip>

namespace yack
{
    namespace chemical
    {

        actor:: ~actor() noexcept
        {
        }

        actor:: actor(const species &sr, const unsigned cf) noexcept :
        next(0),
        prev(0),
        sp(sr),
        nu(cf),
        nm(nu-1)
        {
            assert(nu>0);
            //assert(nm>=0);
        }

        bool actor:: is_primary() const noexcept
        {
            return (1 == sp.rank);
        }


        const species & actor:: operator*() const noexcept { return sp; }

        void actor:: display_first(std::ostream &os) const
        {
            if(nu>1)
            {
                os << nu;
            }
            os << '[' << sp.name << ']';
        }


        void actor:: display_extra(std::ostream &os) const
        {
            os << '+';
            if(nu>1)
            {
                os << nu;
            }
            os << '[' << sp.name << ']';
        }

        string actor:: to_first() const
        {
            string res;
            if(nu>1) {
                res += vformat("%u",nu);
            }
            res += sp.to_string();
            return res;
        }

        string actor:: to_extra() const
        {
            string res = '+';
            if(nu>1) {
                res += vformat("%u",nu);
            }
            res += sp.to_string();
            return res;
        }



        void actor:: display_compact(std::ostream &os, const readable<double> &C) const
        {
            //os << '[' << sp.name << ']' << '=' << std::setw(15) << C[*sp];
            os << '[' << sp.name << ']' << '=' << C[*sp];
        }


    }

}

