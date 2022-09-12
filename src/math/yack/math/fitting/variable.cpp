
#include "yack/math/fitting/variable.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            variable:: ~variable() throw()
            {
                
            }

            variable:: variable(const string &id) : object(), name(id)
            {
            }

            const string & variable:: key() const throw() {
                return name;
            }

            size_t variable:: operator*() const throw()
            {
                assert(indx()>0);
                return indx();
            }


            bool variable:: is_replica() const throw() { return !is_primary(); }


            std::ostream & operator<<(std::ostream &os, const variable &v)
            {
                os << v.name;
                if(v.is_replica())
                {
                    os << "[=" << v.alias() << "]";
                }
                os << "@" << *v;
                return os;
            }

        }

    }

}
