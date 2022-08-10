
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

        size_t library:: size() const throw()
        {
            return (*sdb.tree).size;
        }


        void library:: use(species *s)
        {
            assert(s);
            const species::pointer sp(s);
            assert(**s==size()+1);
            if(!sdb.insert(sp)) throw imported::exception(clid,"multiple species '%s'", s->name() );
            absorb(*sp);
        }

        std::ostream & operator<<(std::ostream  &os,
                                  const library &lib)
        {
            os << '{' << std::endl;
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                lib.pad(os << '[' << s.name <<']',s) << " : z=" << std::setw(5) << s.z << std::endl;
            }
            os << '}';
            return os;
        }


    }

}
