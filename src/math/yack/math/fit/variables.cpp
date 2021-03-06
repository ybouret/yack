
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

            variables & variables::operator<<(const variable &var)
            {
                return (*this)(var.name,var);
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

            static inline void align_to(const size_t w, string &tmp)
            {
                for(size_t j=tmp.size();j<w;++j) tmp +=  ' ';
            }

            void variables:: align_all(strings &out)
            {
                const size_t n = out.size();
                size_t       w = 0;

                for(size_t i=n;i>0;--i)
                    w = max_of(w,out[i].size());

                for(size_t i=n;i>0;--i)
                    align_to(w,out[i]);
            }

            bool variables::is_sep(const int c) throw() { return ':' == c; }


        }
    }
}


