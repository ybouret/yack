#include "yack/chem/vat/cluster/tribe.hpp"

namespace yack
{
    namespace Chemical
    {

        Tribe:: Tribe(const Equilibrium &first) :
        object(),
        Tribe_(),
        lib(),
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
        lib(),
        next(0),
        prev(0)
        {
            (*this) << extra;
        }

        std::ostream & operator<<( std::ostream &os, const Tribe &tribe)
        {
            os << '[';
            const Equilibrium::Node *node = tribe.head;
            os << (***node);
            for(node=node->next;node;node=node->next)
            {
                os << ',' << (***node);
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

            for(const Equilibrium::Node *node=head;node;node=node->next)
            {
                const Equilibrium &scan = ***node;
                const size_t       jndx = scan.indx[SubLevel];
                assert(jndx<indx);
                if(!separated[indx][jndx]) return false;
            }

            return true;
        }

        void Tribe:: finalize()
        {
            addrbook db;
            for(const Equilibrium::Node *node=head;node;node=node->next)
            {
                (***node).submitTo(db);
            }
            for(addrbook::const_iterator it=db.begin();it!=db.end();++it)
            {
                coerce(lib) << *static_cast<const Species *>(*it);
            }
        }


    }

}


