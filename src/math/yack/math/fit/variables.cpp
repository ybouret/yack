
#include "yack/math/fit/variables.hpp"
#include "yack/exception.hpp"
namespace yack
{
    namespace math
    {

        namespace fit
        {
            variables:: ~variables() throw()
            {}

            variables:: variables() throw() : variables_()
            {
            }

            variables:: variables(const variables &other) : variables_(other) {}

            variables & variables:: operator=( const variables &other )
            {
                variables_ &self = *this;
                self = other;
                return *this;
            }



            void variables:: grow(const handle &h)
            {
                if(!insert(h)) throw exception("multiple variable '%s'", h.key()() );
            }


        }
    }
}


