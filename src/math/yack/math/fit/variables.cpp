
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


            size_t variables:: width() const throw()
            {
                size_t w = 0;
                for(const_iterator it=begin();it!=end();++it)
                {
                    const variable &v = **it;
                    w = max_of(w,v.name.size());
                }
                return w;
            }


            size_t variables:: span() const throw()
            {
                size_t res = 0;
                for(const_iterator it=begin();it!=end();++it)
                {
                    res = max_of(res,***it);
                }
                return res;
            }


            bool variables:: handles(const size_t iparam) const throw()
            {
                for(const_iterator it=begin();it!=end();++it)
                {
                    if(iparam == ***it) return true;
                }
                return false;
            }

        }
    }
}


