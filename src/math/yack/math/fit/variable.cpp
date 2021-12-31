
#include "yack/math/fit/variable.hpp"
#include "yack/exception.hpp"

namespace yack
{
    namespace math
    {
        namespace fit
        {
            variable:: ~variable() throw()
            {
            }
            
            const string & variable:: key() const throw() { return name; }

            size_t variable:: operator*() const throw() { return get_index(); }


            void variable:: check(const size_t n) const
            {
                const size_t indx = **this;
                if(indx<1||indx>n) throw exception("variable '%s'@%u not in [1:%u]", name(), (unsigned)indx, (unsigned)n );
            }

            std::ostream & operator<<(std::ostream &os, const variable &v)
            {
                os << v.name << '@' << *v;
                return os;
            }
        }
    }

}

