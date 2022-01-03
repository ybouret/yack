
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

            variables:: variables(const variables &other) : collection(), object(), variables_(other) {}

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


            const variable & variables::fetch(const string &id) const
            {
                const handle *ptr = search(id);
                if(!ptr) throw exception("no variable '%s'", id() );
                return **ptr;
            }

            const variable & variables::fetch(const char *id) const
            {
                const string _(id); return fetch(_);
            }


        }
    }
}


