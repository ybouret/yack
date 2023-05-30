
#include "yack/chem/library.hpp"
#include "yack/system/imported.hpp"
#include <iomanip>

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

        double Library:: Conc(randomized::bits &ran, const double probaNeg) noexcept
        {
            const double p   = double(pmin) + ran.to<double>() * double(pmax-pmin);
            const double ans = pow(10.0,p);
            return ( ran.choice() < probaNeg ) ? -ans : ans;
        }

        void Library:: Conc(writable<double> &C, randomized::bits &ran, const double probaNeg) noexcept
        {
            for(size_t i=C.size();i>0;--i) C[i] = Conc(ran,probaNeg);
        }

        const sList * Library:: operator->() const noexcept
        {
            return & *sdb.get_tree();
        }

        std::ostream & operator<<(std::ostream &os, const Library &lib)
        {
            os << '{' << std::endl;
            for(const sNode *node=lib->head;node;node=node->next)
            {
                const Species &sp = ***node;
                lib.pad(os << ' ' << sp.name,sp) << " : z = " << std::setw(3) << sp.z << std::endl;
            }
            os << '}';
            return os;
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
