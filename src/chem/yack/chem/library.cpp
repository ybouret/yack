

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

        library:: library() throw() : db()
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

        size_t library:: active() const throw()
        {
            size_t ans = 0;
            for(const snode *node=head();node;node=node->next)
            {
                if( (***node).rank > 0 ) ++ans;
            }
            return ans;
        }


        

        size_t library:: primary() const throw()
        {
            size_t ans = 0;
            for(const snode *node=head();node;node=node->next)
            {
                if( 1 == (***node).rank ) ++ans;
            }
            return ans;
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

        const species & library:: operator[](const size_t indx) const throw()
        {
            assert(indx>0);
            assert(indx<=size());
            const snode *node = (*db.tree).get(indx);
            return ***node;
        }


        library:: library(const library &lib) : compendium(), db()
        {
            library &self = *this;
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                self(s.name,s.z);
            }

            assert(self.size()==lib.size());

        }

    }

}

#include "yack/randomized/bits.hpp"

namespace yack
{
    namespace chemical
    {
        void  library:: fill(writable<double> &C,
                             const double      zero_above,
                             randomized::bits &ran) const throw()
        {
            for(const snode *node=head();node;node=node->next)
            {
                const species &s = ***node;
                const size_t   j = *s;

                if( ran.to<double>() >= zero_above)
                {
                    C[j] = 0;
                }
                else
                {
                    C[j] = species::concentration(ran);
                    if(s.rank<=0 && ran.choice()) C[j] = -C[j];
                }
            }
        }
    }

}
