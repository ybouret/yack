
#include "yack/chem/library.hpp"
#include "yack/system/exception.hpp"
#include <iomanip>

namespace yack
{
    namespace chemical
    {

        const char library:: clid[] = "chemical::library";

        library:: ~library() throw()
        {
        }

        library:: library() throw() : gathering()
        {
        }

        library:: library(const library &other) : gathering()
        {

            for(const snode *node=other.head();node;node=node->next)
            {
                use( new species(***node) );
            }

            assert(maxlen==other.maxlen);

        }



        const snode * library:: head() const throw()
        {
            return (*sdb.tree).head;
        }

        library::const_type & library:: operator[](const size_t i) const throw()
        {
           return *** (*sdb.tree).get(i);
        }

        size_t library:: size() const throw()
        {
            return (*sdb.tree).size;
        }


        const species & library:: use(species *s)
        {
            assert(s);
            const species::pointer sp(s);
            assert(**s==size()+1);
            if(!sdb.insert(sp)) throw imported::exception(clid,"multiple species '%s'", s->name() );
            absorb(*sp);
            return *sp;
        }

        std::ostream & operator<<(std::ostream  &os,
                                  const library &lib)
        {
            os << '{' << std::endl;
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                lib.pad(os << '[' << s.name <<']',s);
                os << " | # "    << std::setw(5) << *s;
                os << " | z="    << std::setw(5) << s.z;
                os << " | rank=" << std::setw(5) << s.rank;
                os << std::endl;
            }
            os << '}';
            return os;
        }


        const species * library:: look_up(const string &uid) const throw()
        {
            const species::pointer *pps = sdb.search(uid);
            return pps ? & **pps : NULL;
        }

        const species & library:: operator[](const string &uid) const
        {
            const species::pointer *pps = sdb.search(uid);
            if(!pps) throw imported::exception(clid,"no '%s'",uid());
            return **pps;
        }

        const species & library:: operator[](const char *uid) const
        {
            const string _(uid);
            return (*this)[_];
        }


    }

}
