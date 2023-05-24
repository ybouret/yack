
#include "yack/chem/library.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    namespace Chemical
    {

        const char Library:: CLID[] = "Chemical::Libary";

        Library:: Library() noexcept : Gathering(), sdb()
        {
            
        }

        Library:: ~Library() noexcept
        {

        }

        const sList * Library:: operator->() const noexcept
        {
            return & *sdb.get_tree();
        }

        const Species & Library:: inserted(Species *sp)
        {
            const Species::Pointer  lhs(sp);
            const Species::Pointer *ppS = sdb.search(sp->name);
            if( ppS )
            {
                // already exist
                const Species::Pointer &rhs = *ppS;
                assert(lhs->name == rhs->name);
                if(rhs->z != lhs->z) throw imported::exception(CLID,"charge mismatch for '%s'", sp->name());
                return *rhs;
            }
            else
            {
                // new species
                if(!sdb.insert(lhs)) throw imported::exception(CLID,"unexpected failure to add '%s'", sp->name());
                update(*lhs);
                coerce(lhs->indx[TopLevel]) = sdb.size();
                return *lhs;
            }

        }

        const Species * Library:: query(const string &name) noexcept
        {
            const Species::Pointer *ppS = sdb.search(name);
            if(ppS)
            {
                return & **ppS;
            }
            else
            {
                return NULL;
            }
        }

        const Species * Library:: query(const char   *name)
        {
            const string _(name);
            return query(_);
        }



    }

}
