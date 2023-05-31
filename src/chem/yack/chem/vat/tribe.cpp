#include "yack/chem/vat/tribe.hpp"

namespace yack
{
    namespace Chemical
    {

        Tribe:: Tribe(const Equilibrium &first) :
        object(),
        Tribe_(),
        next(0),
        prev(0)
        {
            (*this) << first;
        }

        Tribe:: ~Tribe() noexcept
        {
        }

        Tribe:: Tribe(const Tribe       &other,
                      const Equilibrium &extra) :
        object(),
        Tribe_(other),
        next(0),
        prev(0)
        {
            (*this) << extra;
        }

        std::ostream & operator<<( std::ostream &os, const Tribe &tribe)
        {
            os << '[';
            const TribeNode *node = tribe.head;
            os << (***node).name;
            for(node=node->next;node;node=node->next)
            {
                os << ',' << (***node).name;
            }
            os << ']';
            return os;
        }


        bool Tribe:: accepts(const Equilibrium  &curr,
                             const matrix<bool> &separated) const noexcept
        {
            assert(size>0);
            assert(NULL!=tail);
            const size_t indx = curr.indx[SubLevel];
            {
                const Equilibrium &last = ***tail;
                if(indx<=last.indx[SubLevel]) return false;
            }

            for(const TribeNode *node=head;node;node=node->next)
            {
                const Equilibrium &scan = ***node;
                const size_t       jndx = scan.indx[SubLevel];
                assert(jndx<indx);
                if(!separated[indx][jndx]) return false;
            }

            return true;
        }
    }

}

namespace yack
{
    namespace Chemical
    {

        Tribes::  Tribes() noexcept : Tribes_() {}
        Tribes:: ~Tribes() noexcept {}

    }

}

