

#include "yack/chem/library.hpp"
#include "yack/exception.hpp"
#include <iomanip>

namespace yack
{
    namespace chemical
    {
        const char library:: clid[] = "chemical::library";

        library:: ~library() throw()
        {
        }

        library:: library() throw()
        {
        }


        size_t library:: size() const throw()
        {
            return db.size();
        }

        const snode * library:: head() const throw()
        {
            return (*db.tree).head;
        }

        

        const species & library:: operator()(const string &name, const int z)
        {
            species_set::const_type *pps = db.search(name);
            if(pps)
            {
                const species &s = **pps;
                if(s.z!=z) throw exception("%s mismatch '%s' charge",clid,name());
                return s;
            }
            else
            {
                const species::pointer p = new species(name,z,size()+1);
                if(!db.insert(p)) throw exception("%s failure to use '%s'",clid,name());
                update_with(*p);
                return *p;
            }
        }

        const species & library:: operator()(const char   *name, const int z)
        {
            const string _(name); return (*this)(_,z);
        }


        std::ostream & operator<<(std::ostream &os, const library &lib)
        {
            os << '{' << std::endl;
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                lib.pad(os << ' ' << s.name,s) << " : @" << *s;
                std::cerr << " | z=" << std::setw(3) << s.z;
                std::cerr << " | rank=" << std::setw(3) << s.rank;
                os << std::endl;
            }
            os << '}';
            return os;
        }
    }

}
