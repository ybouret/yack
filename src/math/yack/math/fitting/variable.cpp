
#include "yack/math/fitting/variable.hpp"

namespace yack
{
    namespace math
    {

        namespace fitting
        {

            variable:: ~variable() noexcept
            {
                
            }

            variable:: variable(const string &id) : object(), name(id)
            {
            }

            const string & variable:: key() const noexcept {
                return name;
            }

            size_t variable:: operator*() const noexcept
            {
                assert(indx()>0);
                return indx();
            }


            bool variable:: is_replica() const noexcept { return !is_primary(); }


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
